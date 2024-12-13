#include "cdk.h"
#include "extreme.h"

FILE* pcm;
void data_cb(extreme_av_buffer_t* data) {
	fwrite(data->data, data->size, 1, pcm);
	free(data);
	data = NULL;
}

int main(void) {
	const int duration = 10000;
	const uint64_t timebase = cdk_time_now();
	pcm = fopen("audio.pcm", "ab+");

	extreme_av_capturer_config_t config = {
		.audio = {
			.format = EXTREME_AUDIO_FORMAT_S16LE,
			.rate = EXTREME_AUDIO_SAMPLERATE_44100HZ,
			.channels = EXTREME_AUDIO_CHANNEL_STEREO,
			.mode = EXTREME_AUDIO_CAPTURER_MODE_DESKTOP,
			.type = EXTREME_AUDIO_CAPTURER_TYPE_PULSEAUDIO
		},
		.video = {
		
		}
	};
	extreme_av_capturer_t* capturer = extreme_av_capturer_create(&config);
	while (cdk_time_now() - timebase <= duration) {
		extreme_av_capturer_capture(capturer, data_cb);
	}
	extreme_av_capturer_destroy(capturer);
	fclose(pcm);
	pcm = NULL;
	return 0;
}