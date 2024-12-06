cmake_minimum_required(VERSION 3.16)

project(main LANGUAGES C)

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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64" OR CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
		set(CDK_LIB_DIR "third-party/cdk/lib/linux/amd64")
	endif()
endif()

include_directories(third-party/cdk/include)
link_directories(${CDK_LIB_DIR})
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG})
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})

add_executable(main "main.c" )
target_link_libraries(main PUBLIC m cdk pa-audio-capturer)

add_subdirectory(pa-audio-capturer)