_Pragma("once")

#include <stdint.h>

typedef enum platform_audio_format_e                platform_audio_format_t;
typedef enum platform_audio_samplerate_e            platform_audio_samplerate_t;
typedef enum platform_audio_channel_e               platform_audio_channel_t;
typedef enum platform_audio_capturer_mode_e         platform_audio_capturer_mode_t;
typedef enum platform_audio_capturer_type_e         platform_audio_capturer_type_t;
typedef struct platform_audio_capturer_config_s     platform_audio_capturer_config_t;

typedef enum platform_video_capturer_type_e         platform_video_capturer_type_t;
typedef struct platform_video_capturer_config_s     platform_video_capturer_config_t;

typedef struct platform_av_capturer_s               platform_av_capturer_t;
typedef struct platform_av_buffer_s                 platform_av_buffer_t;
typedef struct platform_av_capturer_config_s        platform_av_capturer_config_t;

typedef void (*platform_av_capturer_data_cb)(platform_av_buffer_t* data);

enum platform_audio_format_e {
	PLATFORM_AUDIO_FORMAT_NONE,
	PLATFORM_AUDIO_FORMAT_ALAW,
	PLATFORM_AUDIO_FORMAT_S16LE,
	PLATFORM_AUDIO_FORMAT_END
};

enum platform_audio_samplerate_e {
	PLATFORM_AUDIO_SAMPLERATE_NONE,
	PLATFORM_AUDIO_SAMPLERATE_44100HZ,
	PLATFORM_AUDIO_SAMPLERATE_48000HZ,
	PLATFORM_AUDIO_SAMPLERATE_END
};

enum platform_audio_channel_e {
	PLATFORM_AUDIO_CHANNEL_NONE,
	PLATFORM_AUDIO_CHANNEL_MONO,
	PLATFORM_AUDIO_CHANNEL_STEREO,
	PLATFORM_AUDIO_CHANNEL_END
};

enum platform_audio_capturer_mode_e {
	PLATFORM_AUDIO_CAPTURER_MODE_NONE,
	PLATFORM_AUDIO_CAPTURER_MODE_DESKTOP,
	PLATFORM_AUDIO_CAPTURER_MODE_MIC,
	PLATFORM_AUDIO_CAPTURER_MODE_END
};

enum platform_audio_capturer_type_e {
	PLATFORM_AUDIO_CAPTURER_TYPE_NONE,
	PLATFORM_AUDIO_CAPTURER_TYPE_PULSEAUDIO,
	PLATFORM_AUDIO_CAPTURER_TYPE_ALSA,
	PLATFORM_AUDIO_CAPTURER_TYPE_WASAPI,
	PLATFORM_AUDIO_CAPTURER_TYPE_END
};

enum platform_video_capturer_type_e {
	PLATFORM_VIDEO_CAPTURER_TYPE_NONE,
	PLATFORM_VIDEO_CAPTURER_TYPE_X11,
	PLATFORM_VIDEO_CAPTURER_TYPE_KMS,
	PLATFORM_VIDEO_CAPTURER_TYPE_DXGI,
	PLATFORM_VIDEO_CAPTURER_TYPE_END
};

struct platform_audio_capturer_config_s {
	platform_audio_format_t format;
	platform_audio_samplerate_t rate;
	platform_audio_channel_t channels;
	platform_audio_capturer_mode_t mode;
	platform_audio_capturer_type_t type;
};

struct platform_video_capturer_config_s {
	platform_video_capturer_type_t type;
};

struct platform_av_buffer_s {
	uint64_t timestamp;
	int size;
	uint8_t data[];
};

struct platform_av_capturer_s {
	void* audio;
	void* video;
	platform_audio_capturer_type_t at;
	platform_video_capturer_type_t vt;
};

struct platform_av_capturer_config_s {
	platform_audio_capturer_config_t audio;
	platform_video_capturer_config_t video;
};