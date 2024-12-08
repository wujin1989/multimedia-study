cmake_minimum_required(VERSION 3.16)

project(extreme-audio-capturer LANGUAGES C)

set(SRCS
	extreme-audio-capturer.c
)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	list(APPEND SRCS 
		platform/linux-audio-capturer.c
		platform/pulseaudio-capturer.c
	)
	add_library(extreme-audio-capturer STATIC ${SRCS})
	target_link_libraries(extreme-audio-capturer PUBLIC pulse-simple pulse)
endif()
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
endif()
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
endif()
if (CMAKE_SYSTEM_NAME MATCHES "Android")
endif()
if (CMAKE_SYSTEM_NAME MATCHES "iOS")
endif()

