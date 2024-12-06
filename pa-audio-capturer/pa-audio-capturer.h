_Pragma("once")

#include "cdk.h"
#include <pulse/pulseaudio.h>

typedef struct pa_audio_capturer_s pa_audio_capturer_t;

typedef enum pa_audio_capturer_format_e {
	PA_AUDIO_CAPTURER_SAMPLE_S16LE
}pa_audio_capturer_format_t;

typedef struct pa_audio_capturer_config_s {
	pa_audio_capturer_format_t format;
	uint32_t rate;
	uint8_t channels;
}pa_audio_capturer_config_t;

typedef void (*pa_audio_capturer_data_cb)(uint8_t* data, size_t size);

extern pa_audio_capturer_t* pa_audio_capturer_create(pa_audio_capturer_config_t* config);
extern int pa_audio_capturer_capture(pa_audio_capturer_t* capturer, pa_audio_capturer_data_cb cb);
extern void pa_audio_capturer_destroy(pa_audio_capturer_t* capturer);

