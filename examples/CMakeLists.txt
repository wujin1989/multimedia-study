cmake_minimum_required(VERSION 3.16)

project(examples LANGUAGES C)

add_executable(example-server "example-server.c")
target_link_libraries(example-server PUBLIC cdk extreme)

add_executable(example-client "example-client.c")
target_link_libraries(example-client PUBLIC cdk extreme)