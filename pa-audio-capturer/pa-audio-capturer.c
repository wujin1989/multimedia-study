#include "pa-audio-capturer.h"

#define PA_APP_NAME "pa_audio_capturer"
#define PA_STREAM_NAME "pa_audio_record"
#define PA_DEFAULT_MONITOR_NAME "@DEFAULT_MONITOR@"

struct pa_audio_capturer_s {
	pa_threaded_mainloop* ml;
	pa_mainloop_api* api;
	pa_context* ctx;
	pa_stream* stm;
	int fragment_size;
	int frame_size;
};

typedef enum pa_audio_ctx_state_e {
	PULSE_CONTEXT_INITIALIZING,
	PULSE_CONTEXT_READY,
	PULSE_CONTEXT_FINISHED
}pa_audio_ctx_state_t;

static void pa_state_cb(pa_context* c, void* userdata) {
	pa_audio_ctx_state_t* context_state = userdata;

	switch (pa_context_get_state(c)) {
	case PA_CONTEXT_FAILED:
	case PA_CONTEXT_TERMINATED:
		*context_state = PULSE_CONTEXT_FINISHED;
		break;
	case PA_CONTEXT_READY:
		*context_state = PULSE_CONTEXT_READY;
		break;
	default:
		break;
	}
}

static void pa_state_cb2(pa_context* c, void* userdata) {
	pa_threaded_mainloop* mainloop = userdata;

	switch (pa_context_get_state(c)) {
	case PA_CONTEXT_READY:
	case PA_CONTEXT_TERMINATED:
	case PA_CONTEXT_FAILED:
		pa_threaded_mainloop_signal(mainloop, 0);
		break;
	}
}

static void stream_state_cb(pa_stream* s, void* userdata) {
	pa_threaded_mainloop* mainloop = userdata;

	switch (pa_stream_get_state(s)) {
	case PA_STREAM_READY:
	case PA_STREAM_FAILED:
	case PA_STREAM_TERMINATED:
		pa_threaded_mainloop_signal(mainloop, 0);
		break;
	}
}

static void stream_request_cb(pa_stream* s, size_t length, void* userdata) {
	pa_threaded_mainloop* mainloop = userdata;
	pa_threaded_mainloop_signal(mainloop, 0);
}

static void stream_latency_update_cb(pa_stream* s, void* userdata) {
	pa_threaded_mainloop* mainloop = userdata;
	pa_threaded_mainloop_signal(mainloop, 0);
}

static void pulse_audio_source_device_cb(pa_context* c, const pa_source_info* dev, int eol, void* userdata) {
	if (!eol) {
		printf("source name: %s\n", dev->name);
	}
}

static void pulse_server_info_cb(pa_context* c, const pa_server_info* i, void* userdata) {
	printf("default source name: %s\n", i->default_source_name);
}

static void get_monitor_device(char* dev) {
	pa_audio_capturer_t* capturer = NULL;
	pa_mainloop* pa_ml = NULL;
	pa_mainloop_api* pa_mlapi = NULL;
	pa_context* pa_ctx = NULL;
	pa_operation* pa_op = NULL;
	char* monitor_device = NULL;
	pa_operation_state_t op_state;
	pa_audio_ctx_state_t pa_ctx_state = PULSE_CONTEXT_INITIALIZING;

	pa_ml = pa_mainloop_new();
	if (!pa_ml) {
		goto fail;
	}
	pa_mlapi = pa_mainloop_get_api(pa_ml);
	if (!pa_mlapi) {
		goto fail;
	}
	pa_ctx = pa_context_new(pa_mlapi, PA_APP_NAME);
	if (!pa_ctx) {
		goto fail;
	}
	pa_context_set_state_callback(pa_ctx, pa_state_cb, &pa_ctx_state);
	if (pa_context_connect(pa_ctx, NULL, 0, NULL) < 0) {
		goto fail;
	}
	while (pa_ctx_state == PULSE_CONTEXT_INITIALIZING) {
		pa_mainloop_iterate(pa_ml, 1, NULL);
	}
	if (pa_ctx_state == PULSE_CONTEXT_FINISHED) {
		goto fail;
	}
	pa_op = pa_context_get_source_info_list(pa_ctx, pulse_audio_source_device_cb, dev);
	while ((op_state = pa_operation_get_state(pa_op)) == PA_OPERATION_RUNNING) {
		pa_mainloop_iterate(pa_ml, 1, NULL);
	}
	pa_operation_unref(pa_op);
	pa_op = pa_context_get_server_info(pa_ctx, pulse_server_info_cb, dev);
	while ((op_state = pa_operation_get_state(pa_op)) == PA_OPERATION_RUNNING)
		pa_mainloop_iterate(pa_ml, 1, NULL);
	pa_operation_unref(pa_op);
	return;
fail:
	if (pa_ctx) {
		pa_context_set_state_callback(pa_ctx, NULL, NULL);
		pa_context_disconnect(pa_ctx);
		pa_context_unref(pa_ctx);
		pa_ctx = NULL;
	}
	if (pa_ml) {
		pa_mainloop_free(pa_ml);
		pa_ml = NULL;
	}
	return;
}

pa_audio_capturer_t* pa_audio_capturer_create(pa_audio_capturer_config_t* config){
	/*char monitor[128] = { 0 };
	get_monitor_device(monitor);*/
	int ret;
	const pa_buffer_attr* queried_attr;
	pa_buffer_attr attr = { -1 };
	pa_channel_map cmap;
	pa_sample_spec ss;
	pa_audio_capturer_t* capturer = NULL;

	capturer = malloc(sizeof(pa_audio_capturer_t));
	if (!capturer) {
		goto fail;
	}
	ss.channels = config->channels;
	ss.format = config->format;
	ss.rate = config->rate;

	pa_channel_map_init_extend(&cmap, config->channels, PA_CHANNEL_MAP_WAVEEX);
	attr.fragsize = pa_frame_size(&ss) * (config->rate / 20); //50ms latency

	pa_threaded_mainloop* mainloop = NULL;
	if (!(mainloop = pa_threaded_mainloop_new())) {
		goto fail;
		return NULL;
	}
	pa_mainloop_api* mainloop_api = NULL;
	if (!(mainloop_api = pa_threaded_mainloop_get_api(mainloop))) {
		goto fail;
	}
	pa_context* context = NULL;
	if (!(context = pa_context_new(mainloop_api, PA_APP_NAME))) {
		goto fail;
	}
	pa_context_set_state_callback(context, pa_state_cb2, mainloop);

	if (pa_context_connect(context, NULL, 0, NULL) < 0) {
		goto fail;
	}
	pa_threaded_mainloop_lock(mainloop);
	if (pa_threaded_mainloop_start(mainloop) < 0) {
		pa_threaded_mainloop_unlock(mainloop);
		goto fail;
	}
	for (;;) {
		pa_context_state_t state;
		state = pa_context_get_state(context);
		if (state == PA_CONTEXT_READY) {
			break;
		}
		if (!PA_CONTEXT_IS_GOOD(state)) {
			pa_threaded_mainloop_unlock(mainloop);
			goto fail;
		}
		pa_threaded_mainloop_wait(mainloop);
	}
	pa_stream* stream = NULL;
	if (!(stream = pa_stream_new(context, PA_STREAM_NAME, &ss, &cmap))) {
		pa_threaded_mainloop_unlock(mainloop);
		goto fail;
	}
	pa_stream_set_state_callback(stream, stream_state_cb, mainloop);
	pa_stream_set_read_callback(stream, stream_request_cb, mainloop);
	pa_stream_set_write_callback(stream, stream_request_cb, mainloop);
	pa_stream_set_latency_update_callback(stream, stream_latency_update_cb, mainloop);

	ret = pa_stream_connect_record(stream, 
									PA_DEFAULT_MONITOR_NAME, 
									&attr,
									PA_STREAM_INTERPOLATE_TIMING
									| PA_STREAM_ADJUST_LATENCY
									| PA_STREAM_AUTO_TIMING_UPDATE);
	if (ret < 0) {
		pa_threaded_mainloop_unlock(mainloop);
		goto fail;
	}
	for (;;) {
		pa_stream_state_t state;
		state = pa_stream_get_state(stream);
		if (state == PA_STREAM_READY) {
			break;
		}
		if (!PA_STREAM_IS_GOOD(state)) {
			pa_threaded_mainloop_unlock(mainloop);
			goto fail;
		}
		pa_threaded_mainloop_wait(mainloop);
	}
	queried_attr = pa_stream_get_buffer_attr(stream);
	if (!queried_attr || queried_attr->fragsize > INT_MAX / 100) {
		pa_threaded_mainloop_unlock(mainloop);
		goto fail;
	}
	pa_threaded_mainloop_unlock(mainloop);

	capturer->api = mainloop_api;
	capturer->ctx = context;
	capturer->ml = mainloop;
	capturer->fragment_size = queried_attr->fragsize;
	capturer->frame_size = pa_frame_size(&ss);
	capturer->stm = stream;
	return capturer;
fail:
	if (mainloop) {
		pa_threaded_mainloop_stop(mainloop);
	}
	if (stream) {
		pa_stream_unref(stream);
		stream = NULL;
	}
	if (context) {
		pa_context_disconnect(context);
		pa_context_unref(context);
		context = NULL;
	}
	if (mainloop) {
		pa_threaded_mainloop_free(mainloop);
		mainloop = NULL;
	}
	return NULL;
}

int pa_audio_capturer_capture(pa_audio_capturer_t* capturer, pa_audio_capturer_data_cb cb) {
	int ret;
	size_t read_length;
	const void* read_data = NULL;
	int64_t dts;
	pa_usec_t latency;
	int negative;
	ptrdiff_t pos = 0;

	pa_threaded_mainloop_lock(capturer->ml);
	while (pos < capturer->fragment_size) {
		int r;
		r = pa_stream_peek(capturer->stm, &read_data, &read_length);
		if (r) {
			goto fail;
		}
		if (read_length <= 0) {
			pa_threaded_mainloop_wait(capturer->ml);
		}
		else if (!read_data) {
			r = pa_stream_drop(capturer->stm);
			if (r) {
				goto fail;
			}
		}
		else {
			if (!pos) {
				if (av_new_packet(pkt, capturer->fragment_size) < 0) {
					ret = AVERROR(ENOMEM);
					goto unlock_and_fail;
				}
				dts = av_gettime();
				pa_operation_unref(pa_stream_update_timing_info(capturer->stm, NULL, NULL));

				if (pa_stream_get_latency(capturer->stm, &latency, &negative) >= 0) {
					if (negative) {
						dts += latency;
					} else {
						dts -= latency;
					}
				}
			}
			if (pkt->size - pos < read_length) {
				if (pos)
					break;
				pa_stream_drop(capturer->stm);
				/* Oversized fragment??? */
				ret = AVERROR_EXTERNAL;
				goto unlock_and_fail;
			}
			memcpy(pkt->data + pos, read_data, read_length);
			pos += read_length;
			pa_stream_drop(capturer->stm);
		}
	}
	pa_threaded_mainloop_unlock(capturer->ml);
	return 0;
fail:
	av_packet_unref(pkt);
	pa_threaded_mainloop_unlock(capturer->ml);
	return ret;
}

void pa_audio_capturer_destroy(pa_audio_capturer_t* capturer) {
	if (capturer) {
		free(capturer);
		capturer = NULL;
	}
}