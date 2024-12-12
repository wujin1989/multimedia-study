_Pragma("once")

#include "platform-types.h"

extern platform_av_capturer_t* platform_av_capturer_create(platform_av_capturer_config_t* p_av_c_conf);
extern void platform_av_capturer_capture(platform_av_capturer_t* p_av_c, platform_av_capturer_data_cb cb);
extern void platform_av_capturer_destroy(platform_av_capturer_t* p_av_c);

