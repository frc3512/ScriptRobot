cmake_minimum_required(VERSION 2.8)
set(NAME packager)
project(${NAME})
include_directories(../src/)
include_directories(/src/)

set(CMAKE_CXX_FLAGS "-O3 -Wall -s -std=c++11 -fpermissive -w")
set(CMAKE_C_FLAGS "-O3 -Wall -s -std=c11 -fpermissive -w")

set(DIRS
	src/*
	../src/*
)

file(GLOB SRC *.cpp *.c)

foreach(ELEM ${DIRS})
    file(GLOB_RECURSE FILES ${ELEM}.cpp ${ELEM}.c)
    list(APPEND SRC ${FILES})
endforeach(ELEM)

add_executable(${NAME}.exe ${SRC})


IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
target_link_libraries(${NAME}.exe libAngelscript.a pthread)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
target_link_libraries(${NAME}.exe libAngelscript.a ws2_32)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
