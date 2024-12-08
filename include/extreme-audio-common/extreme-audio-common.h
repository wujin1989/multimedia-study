_Pragma("once")

typedef enum extreme_audio_format_e {
	EXTREME_AUDIO_FORMAT_S16LE,
	EXTREME_AUDIO_FORMAT_END
} extreme_audio_format_t;

typedef enum extreme_audio_samplerate_e {
	EXTREME_AUDIO_SAMPLERATE_44100HZ,
	EXTREME_AUDIO_SAMPLERATE_END
}extreme_audio_samplerate_t;

typedef enum extreme_audio_channel_e {
	EXTREME_AUDIO_CHANNEL_STEREO,
	EXTREME_AUDIO_CHANNEL_END
}extreme_audio_channel_t;