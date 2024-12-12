#include <stdio.h>
#include <stdlib.h>
#include "extreme/extreme-types.h"
#include "platform/platform-av-capturer.h"

static void audio_capturer_config_convert(extreme_audio_capturer_config_t* from, platform_audio_capturer_config_t* to) {
	switch (from->format) {
	case EXTREME_AUDIO_FORMAT_S16LE:
		to->format = PLATFORM_AUDIO_FORMAT_S16LE;
		break;
	default:
		break;
	}
	switch (from->rate) {
	case EXTREME_AUDIO_SAMPLERATE_44100HZ:
		to->rate = PLATFORM_AUDIO_SAMPLERATE_44100HZ;
		break;
	default:
		break;
	}
	switch (from->channels) {
	case EXTREME_AUDIO_CHANNEL_STEREO:
		to->channels = PLATFORM_AUDIO_CHANNEL_STEREO;
		break;
	default:
		break;
	}
	switch (from->mode) {
	case EXTREME_AUDIO_CAPTURER_MODE_DESKTOP:
		to->mode = PLATFORM_AUDIO_CAPTURER_MODE_DESKTOP;
		break;
	default:
		break;
	}
	switch (from->type) {
	case EXTREME_AUDIO_CAPTURER_TYPE_PULSEAUDIO:
		to->type = PLATFORM_AUDIO_CAPTURER_TYPE_PULSEAUDIO;
		break;
	default:
		break;
	}
}

static void video_capturer_config_convert(extreme_video_capturer_config_t* from, platform_video_capturer_config_t* to) {

}

static void av_capturer_config_convert(extreme_av_capturer_config_t* from, platform_av_capturer_config_t* to) {
	audio_capturer_config_convert(&from->audio, &to->audio);
	video_capturer_config_convert(&from->video, &to->video);
}

static extreme_av_capturer_t* av_capturer_convert(platform_av_capturer_t* p_av_c) {
	extreme_av_capturer_t* e_av_c = malloc(sizeof(extreme_av_capturer_t));
	if (!e_av_c) {
		return NULL;
	}
	e_av_c->opaque = p_av_c;
	return e_av_c;
}

extreme_av_capturer_t* extreme_av_capturer_create(extreme_av_capturer_config_t* e_av_c_conf) {
	platform_av_capturer_config_t p_av_c_conf;
	av_capturer_config_convert(e_av_c_conf, &p_av_c_conf);
	return av_capturer_convert(platform_av_capturer_create(&p_av_c_conf));
}

void extreme_av_capturer_capture(extreme_av_capturer_t* e_av_c, extreme_av_capturer_data_cb cb) {
	return platform_av_capturer_capture((platform_av_capturer_t*)e_av_c->opaque, (platform_av_capturer_data_cb)cb);
}

void extreme_av_capturer_destroy(extreme_av_capturer_t* e_av_c) {
	if (e_av_c) {
		platform_av_capturer_destroy((platform_av_capturer_t*)e_av_c->opaque);
		free(e_av_c);
		e_av_c = NULL;
	}
}