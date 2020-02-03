cmake_minimum_required(VERSION 3.15)
project(Chat_Application_Phase1 C)

set(CMAKE_C_STANDARD 99)
link_libraries(ws2_32 wsock32)
add_executable(Chat_Application_Phase1 main.c )
