#include "pulseaudio-capturer.h"
#include "cdk.h"

#define PA_APP_NAME "capturer"
#define PA_STREAM_NAME "record"
#define PA_DEFAULT_MONITOR_NAME "@DEFAULT_MONITOR@"
#define PA_US_PER_MS 1000

struct pulseaudio_capturer_s {
	pa_threaded_mainloop* ml;
	pa_mainloop_api* api;
	pa_context* ctx;
	pa_stream* stm;
	int fragment_size;
	int frame_size;
};

static void pa_state_cb(pa_context* c, void* userdata) {
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

pulseaudio_capturer_t* pulseaudio_capturer_create(pulseaudio_capturer_config_t* pa_c_conf) {
	int ret;
	const pa_buffer_attr* queried_attr;
	pa_buffer_attr attr = { -1 };
	pa_channel_map cmap;
	pa_sample_spec ss;
	pulseaudio_capturer_t* capturer = NULL;

	capturer = malloc(sizeof(pulseaudio_capturer_t));
	if (!capturer) {
		goto fail;
	}
	ss.channels = pa_c_conf->channels;
	ss.format = pa_c_conf->format;
	ss.rate = pa_c_conf->rate;

	pa_channel_map_init_extend(&cmap, pa_c_conf->channels, PA_CHANNEL_MAP_WAVEEX);
	attr.fragsize = pa_frame_size(&ss) * (pa_c_conf->rate / 20); //50ms latency

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
	pa_context_set_state_callback(context, pa_state_cb, mainloop);

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

void pulseaudio_capturer_capture(pulseaudio_capturer_t* pa_c, pulseaudio_capturer_cb cb) {
	size_t rxlen;
	const void* rxdata = NULL;
	pa_usec_t latency;
	int negative;
	ptrdiff_t pos = 0;
	pulseaudio_audio_buffer_t* buffer = NULL;

	pa_threaded_mainloop_lock(pa_c->ml);

	if (!pa_c->ctx
		|| !PA_CONTEXT_IS_GOOD(pa_context_get_state(pa_c->ctx))
		|| !pa_c->stm
		|| !PA_STREAM_IS_GOOD(pa_stream_get_state(pa_c->stm))) {
		goto fail;
	}
	while (pos < pa_c->fragment_size) {
		if (pa_stream_peek(pa_c->stm, &rxdata, &rxlen)) {
			goto fail;
		}
		if (rxlen <= 0) {
			pa_threaded_mainloop_wait(pa_c->ml);
			if (!pa_c->ctx
				|| !PA_CONTEXT_IS_GOOD(pa_context_get_state(pa_c->ctx))
				|| !pa_c->stm
				|| !PA_STREAM_IS_GOOD(pa_stream_get_state(pa_c->stm))) {
				goto fail;
			}
		} else if (!rxdata) {
			if (pa_stream_drop(pa_c->stm)) {
				goto fail;
			}
		} else {
			if (!pos) {
				buffer = malloc(sizeof(pulseaudio_audio_buffer_t) + pa_c->fragment_size);
				if (!buffer) {
					goto fail;
				}
				memset(buffer, 0, sizeof(pulseaudio_audio_buffer_t) + pa_c->fragment_size);
				buffer->size = pa_c->fragment_size;

				pa_operation_unref(pa_stream_update_timing_info(pa_c->stm, NULL, NULL));
				pa_stream_get_latency(pa_c->stm, &latency, &negative);
			}
			if (buffer->size - pos < rxlen) {
				if (pos) {
					break;
				}
				pa_stream_drop(pa_c->stm);
				goto fail;
			}
			memcpy(buffer->data + pos, rxdata, rxlen);
			pos += rxlen;
			pa_stream_drop(pa_c->stm);
		}
	}
	pa_threaded_mainloop_unlock(pa_c->ml);
	if (buffer->size < pos) {
		return;
	}
	buffer->size = pos;
	buffer->timestamp = cdk_time_now();
	if (negative) {
		buffer->timestamp += (latency / PA_US_PER_MS);
	} else {
		buffer->timestamp -= (latency / PA_US_PER_MS);
	}
	cb(buffer);
	return;
fail:
	if (buffer) {
		free(buffer);
		buffer = NULL;
	}
	pa_threaded_mainloop_unlock(pa_c->ml);
	return;
}

void pulseaudio_capturer_destroy(pulseaudio_capturer_t* pa_c) {
	if (pa_c) {
		free(pa_c);
		pa_c = NULL;
	}
}