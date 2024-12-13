cmake_minimum_required(VERSION 3.16)

project(extreme LANGUAGES C)

if (CMAKE_SIZEOF_VOID_P LESS 8)
	message(FATAL_ERROR "only 64bit supported")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD 11)

if(WIN32)
	if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
		add_compile_options("/experimental:c11atomics")
	endif()
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64" OR CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
		set(CDK_LIB_DIR "third-party/cdk/lib/linux/amd64")
		set(OPUS_LIB_DIR "third-party/opus/lib/linux/amd64")
	endif()
endif()

include_directories(include)
include_directories(src)
include_directories(third-party/cdk/include)
include_directories(third-party/opus/include)
link_directories(${CDK_LIB_DIR})
link_directories(${OPUS_LIB_DIR})

set(SRCS
	src/av-capturer/extreme-av-capturer.c
	src/av-encoder/extreme-av-encoder.c
	src/av-encoder/opus-encoder.c
)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	list(APPEND SRCS 
		src/platform/linux/platform-av-capturer.c
		src/platform/linux/pulseaudio-capturer.c
		src/platform/linux/alsa-capturer.c
	)
	add_library(extreme STATIC ${SRCS})
	target_link_libraries(extreme PUBLIC m cdk pulse opus)
endif()

add_subdirectory(examples)