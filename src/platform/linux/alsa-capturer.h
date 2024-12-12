_Pragma("once")

#include <stdint.h>
#include <stddef.h>

typedef struct alsa_capturer_s                         alsa_capturer_t;
typedef struct alsa_capturer_config_s                  alsa_capturer_config_t;
typedef struct alsa_audio_buffer_s                     alsa_audio_buffer_t;

typedef void (*alsa_capturer_cb)(alsa_audio_buffer_t* buffer);

struct alsa_capturer_config_s {

};

struct alsa_audio_buffer_s {
    uint64_t timestamp;
    int size;
    uint8_t data[];
};