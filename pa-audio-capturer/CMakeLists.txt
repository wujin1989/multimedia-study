cmake_minimum_required(VERSION 3.16)

project(pa-audio-capturer LANGUAGES C)

set(SRCS
	pa-audio-capturer.c
)

add_library(pa-audio-capturer STATIC ${SRCS})
target_link_libraries(pa-audio-capturer PUBLIC pulse-simple pulse)