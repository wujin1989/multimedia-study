_Pragma("once")

#include "extreme-audio-capturer/extreme-audio-capturer.h"

extern extreme_audio_capturer_t* platform_audio_capturer_create(extreme_audio_capturer_config_t* config);
extern int platform_audio_capturer_capture(extreme_audio_capturer_t* capturer, extreme_audio_capturer_cb cb);
extern void platform_audio_capturer_destroy(extreme_audio_capturer_t* capturer);

