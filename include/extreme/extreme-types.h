_Pragma("once")

#include <stdint.h>

typedef enum extreme_audio_format_e                   extreme_audio_format_t;
typedef enum extreme_audio_samplerate_e               extreme_audio_samplerate_t;
typedef enum extreme_audio_channel_e                  extreme_audio_channel_t;
typedef enum extreme_audio_capturer_mode_e            extreme_audio_capturer_mode_t;
typedef enum extreme_audio_capturer_type_e            extreme_audio_capturer_type_t;
typedef struct extreme_audio_capturer_config_s        extreme_audio_capturer_config_t;

typedef enum extreme_video_capturer_type_e            extreme_video_capturer_type_t;
typedef struct extreme_video_capturer_config_s        extreme_video_capturer_config_t;

typedef struct extreme_av_capturer_s                  extreme_av_capturer_t;
typedef struct extreme_av_buffer_s                    extreme_av_buffer_t;
typedef struct extreme_av_capturer_config_s           extreme_av_capturer_config_t;

typedef void (*extreme_av_capturer_data_cb)(extreme_av_buffer_t* data);

enum extreme_audio_format_e {
	EXTREME_AUDIO_FORMAT_NONE,
	EXTREME_AUDIO_FORMAT_S16LE,
	EXTREME_AUDIO_FORMAT_END
};

enum extreme_audio_samplerate_e {
	EXTREME_AUDIO_SAMPLERATE_NONE,
	EXTREME_AUDIO_SAMPLERATE_44100HZ,
	EXTREME_AUDIO_SAMPLERATE_END
};

enum extreme_audio_channel_e {
	EXTREME_AUDIO_CHANNEL_NONE,
	EXTREME_AUDIO_CHANNEL_STEREO,
	EXTREME_AUDIO_CHANNEL_END
};

struct extreme_av_capturer_s {
	void* opaque;
};

struct extreme_av_buffer_s {
	uint64_t timestamp;
	int size;
	uint8_t data[];
};

enum extreme_audio_capturer_mode_e {
	EXTREME_AUDIO_CAPTURER_MODE_NONE,
	EXTREME_AUDIO_CAPTURER_MODE_DESKTOP,
	EXTREME_AUDIO_CAPTURER_MODE_END
};

enum extreme_audio_capturer_type_e {
	EXTREME_AUDIO_CAPTURER_TYPE_NONE,
	EXTREME_AUDIO_CAPTURER_TYPE_PULSEAUDIO,
	EXTREME_AUDIO_CAPTURER_TYPE_END
};

enum extreme_video_capturer_type_e {
	EXTREME_VIDEO_CAPTURER_TYPE_NONE,
	EXTREME_VIDEO_CAPTURER_TYPE_X11,
	EXTREME_VIDEO_CAPTURER_TYPE_END
};

struct extreme_audio_capturer_config_s {
	extreme_audio_format_t format;
	extreme_audio_samplerate_t rate;
	extreme_audio_channel_t channels;
	extreme_audio_capturer_mode_t mode;
	extreme_audio_capturer_type_t type;
};

struct extreme_video_capturer_config_s {

};

struct extreme_av_capturer_config_s {
	extreme_audio_capturer_config_t audio;
	extreme_video_capturer_config_t video;
};