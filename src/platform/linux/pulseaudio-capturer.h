_Pragma("once")

#include <stdint.h>
#include <stddef.h>
#include <pulse/pulseaudio.h>

typedef struct pulseaudio_capturer_s                   pulseaudio_capturer_t;
typedef enum pulseaudio_capturer_format_e              pulseaudio_capturer_format_t;
typedef enum pulseaudio_capturer_mode_e                pulseaudio_capturer_mode_t;
typedef struct pulseaudio_capturer_config_s            pulseaudio_capturer_config_t;
typedef struct pulseaudio_audio_buffer_s               pulseaudio_audio_buffer_t;

typedef void (*pulseaudio_capturer_cb)(pulseaudio_audio_buffer_t* buffer);

/**
 * Refer to the structure of pa_sample_format_t and match it item by item
 */
enum pulseaudio_capturer_format_e {
    PULSEAUDIO_CAPTURER_FMT_NONE = -1,
    PULSEAUDIO_CAPTURER_FMT_U8 = 0,
    PULSEAUDIO_CAPTURER_FMT_ALAW,
    PULSEAUDIO_CAPTURER_FMT_ULAW,
    PULSEAUDIO_CAPTURER_FMT_S16LE,
    PULSEAUDIO_CAPTURER_FMT_S16BE,
    PULSEAUDIO_CAPTURER_FMT_FLOAT32LE,
    PULSEAUDIO_CAPTURER_FMT_FLOAT32BE,
    PULSEAUDIO_CAPTURER_FMT_S32LE,
    PULSEAUDIO_CAPTURER_FMT_S32BE,
    PULSEAUDIO_CAPTURER_FMT_S24LE,
    PULSEAUDIO_CAPTURER_FMT_S24BE,
    PULSEAUDIO_CAPTURER_FMT_32LE,
    PULSEAUDIO_CAPTURER_FMT_32BE,
    PULSEAUDIO_CAPTURER_FMT_END
};

enum pulseaudio_capturer_mode_e {
    PULSEAUDIO_CAPTURER_MODE_NONE,
	PULSEAUDIO_CAPTURER_MODE_DESKTOP,
	PULSEAUDIO_CAPTURER_MODE_MIC,
	PULSEAUDIO_CAPTURER_MODE_END
};

struct pulseaudio_capturer_config_s {
	pulseaudio_capturer_format_t format;
	uint32_t rate;
	uint8_t channels;
	pulseaudio_capturer_mode_t mode;
};

struct pulseaudio_audio_buffer_s {
    uint64_t timestamp;
    int size;
    uint8_t data[];
};

extern pulseaudio_capturer_t* pulseaudio_capturer_create(pulseaudio_capturer_config_t* config);
extern void pulseaudio_capturer_capture(pulseaudio_capturer_t* capturer, pulseaudio_capturer_cb cb);
extern void pulseaudio_capturer_destroy(pulseaudio_capturer_t* capturer);