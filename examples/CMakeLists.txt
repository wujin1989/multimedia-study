cmake_minimum_required(VERSION 3.16)

project(examples LANGUAGES C)

add_executable(example-av-capturer "example-av-capturer.c")
target_link_libraries(example-av-capturer PUBLIC cdk extreme)