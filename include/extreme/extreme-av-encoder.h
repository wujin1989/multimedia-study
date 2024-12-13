_Pragma("once")

#include "extreme-types.h"

extern extreme_av_encoder_t* extreme_av_encoder_create(extreme_av_encoder_config_t* e_av_e_conf);
extern void extreme_av_encoder_encode(extreme_av_encoder_t* e_av_e);
extern void extreme_av_encoder_destroy(extreme_av_encoder_t* e_av_e);