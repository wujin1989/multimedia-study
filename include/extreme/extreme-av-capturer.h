_Pragma("once")

#include "extreme-types.h"

extern extreme_av_capturer_t* extreme_av_capturer_create(extreme_av_capturer_config_t* e_av_c_conf);
extern void extreme_av_capturer_capture(extreme_av_capturer_t* e_av_c, extreme_av_capturer_data_cb cb);
extern void extreme_av_capturer_destroy(extreme_av_capturer_t* e_av_c);