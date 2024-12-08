#include "extreme-audio-capturer/extreme-audio-capturer.h"
#include "platform/platform-audio-capturer.h"

extreme_audio_capturer_t* extreme_audio_capturer_create(extreme_audio_capturer_config_t* config) {
	return platform_audio_capturer_create(config);
}

int extreme_audio_capturer_capture(extreme_audio_capturer_t* capturer, extreme_audio_capturer_cb cb) {
	return platform_audio_capturer_capture(capturer, cb);
}

void extreme_audio_capturer_destroy(extreme_audio_capturer_t* capturer) {
	return platform_audio_capturer_destroy(capturer);
}