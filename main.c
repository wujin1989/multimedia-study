#include "cdk.h"
#include "pa-audio-capturer/pa-audio-capturer.h"

int main(void) {
	pa_audio_capturer_t* capturer = NULL;
	pa_audio_capturer_config_t config = {
		.channels = 2,
		.format = PA_AUDIO_CAPTURER_SAMPLE_S16LE,
		.rate = 48000
	};
	capturer = pa_audio_capturer_create(&config);
	return 0;
}