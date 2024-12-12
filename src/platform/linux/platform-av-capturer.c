#include "platform/platform-av-capturer.h"
#include "pulseaudio-capturer.h"
#include "alsa-capturer.h"

static void pulseaudio_capturer_config_convert(platform_audio_capturer_config_t* from, pulseaudio_capturer_config_t* to) {
	switch (from->format) {
	case PLATFORM_AUDIO_FORMAT_S16LE:
		to->format = PULSEAUDIO_CAPTURER_FMT_S16LE;
		break;
	case PLATFORM_AUDIO_FORMAT_ALAW:
		to->format = PULSEAUDIO_CAPTURER_FMT_ALAW;
		break;
	default:
		break;
	}
	switch (from->rate) {
	case PLATFORM_AUDIO_SAMPLERATE_44100HZ:
		to->rate = 44100;
		break;
	case PLATFORM_AUDIO_SAMPLERATE_48000HZ:
		to->rate = 48000;
		break;
	default:
		break;
	}
	switch (from->channels) {
	case PLATFORM_AUDIO_CHANNEL_STEREO:
		to->channels = 2;
		break;
	case PLATFORM_AUDIO_CHANNEL_MONO:
		to->channels = 1;
		break;
	default:
		break;
	}
	switch (from->mode) {
	case PLATFORM_AUDIO_CAPTURER_MODE_DESKTOP:
		to->mode = PULSEAUDIO_CAPTURER_MODE_DESKTOP;
		break;
	case PLATFORM_AUDIO_CAPTURER_MODE_MIC:
		to->mode = PULSEAUDIO_CAPTURER_MODE_MIC;
		break;
	default:
		break;
	}
}

static void alsa_capturer_config_convert(platform_audio_capturer_config_t* from, alsa_capturer_config_t* to) {

}

platform_av_capturer_t* platform_av_capturer_create(platform_av_capturer_config_t* p_av_c_conf) {
	platform_av_capturer_t* p_av_c = malloc(sizeof(platform_av_capturer_t));
	if (!p_av_c) {
		return NULL;
	}
	if (p_av_c_conf->audio.type == PLATFORM_AUDIO_CAPTURER_TYPE_PULSEAUDIO) {
		pulseaudio_capturer_config_t conf;
		pulseaudio_capturer_config_convert(&p_av_c_conf->audio, &conf);

		p_av_c->audio = pulseaudio_capturer_create(&conf);
		p_av_c->at = PLATFORM_AUDIO_CAPTURER_TYPE_PULSEAUDIO;
	}
	if (p_av_c_conf->audio.type == PLATFORM_AUDIO_CAPTURER_TYPE_ALSA) {
		
	}
	if (p_av_c_conf->video.type == PLATFORM_VIDEO_CAPTURER_TYPE_X11) {
		
	}
	if (p_av_c_conf->video.type == PLATFORM_VIDEO_CAPTURER_TYPE_KMS) {
		
	}
	return p_av_c;
}

void platform_av_capturer_capture(platform_av_capturer_t* p_av_c, platform_av_capturer_data_cb cb) {
	if (p_av_c->at == PLATFORM_AUDIO_CAPTURER_TYPE_PULSEAUDIO) {
		return pulseaudio_capturer_capture((pulseaudio_capturer_t*)p_av_c->audio, (pulseaudio_capturer_cb)cb);
	}
	if (p_av_c->at == PLATFORM_AUDIO_CAPTURER_TYPE_ALSA) {
		
	}
	if (p_av_c->vt == PLATFORM_VIDEO_CAPTURER_TYPE_X11) {

	}
	if (p_av_c->vt == PLATFORM_VIDEO_CAPTURER_TYPE_KMS) {

	}
}

void platform_av_capturer_destroy(platform_av_capturer_t* p_av_c) {
	if (p_av_c) {
		if (p_av_c->at == PLATFORM_AUDIO_CAPTURER_TYPE_PULSEAUDIO) {
			pulseaudio_capturer_destroy((pulseaudio_capturer_t*)p_av_c->audio);
		}
		if (p_av_c->at == PLATFORM_AUDIO_CAPTURER_TYPE_ALSA) {
		
		}
		if (p_av_c->vt == PLATFORM_VIDEO_CAPTURER_TYPE_X11) {

		}
		if (p_av_c->vt == PLATFORM_VIDEO_CAPTURER_TYPE_KMS) {

		}
		free(p_av_c);
		p_av_c = NULL;
	}
}