_Pragma("once")

#include "extreme-audio-common/extreme-audio-common.h"
#include <stdint.h>
#include <stddef.h>

typedef struct extreme_audio_capturer_s extreme_audio_capturer_t;
typedef void (*extreme_audio_capturer_cb)(uint8_t* data, size_t size);

typedef enum extreme_audio_capturer_mode_e {
	EXTREME_AUDIO_CAPTURER_MODE_DESKTOP,
	EXTREME_AUDIO_CAPTURER_MODE_MIC,
	EXTREME_AUDIO_CAPTURER_MODE_END
} extreme_audio_capturer_mode_t;

typedef struct extreme_audio_capturer_config_s {
	extreme_audio_format_t format;
	extreme_audio_samplerate_t rate;
	extreme_audio_channel_t channels;
	extreme_audio_capturer_mode_t mode;
}extreme_audio_capturer_config_t;

extern extreme_audio_capturer_t* extreme_audio_capturer_create(extreme_audio_capturer_config_t* config);
extern int extreme_audio_capturer_capture(extreme_audio_capturer_t* capturer, extreme_audio_capturer_cb cb);
extern void extreme_audio_capturer_destroy(extreme_audio_capturer_t* capturer);