cmake_minimum_required(VERSION 2.8.8)
project(mmmatrixio)

message(STATUS "System is " ${CMAKE_SYSTEM_NAME})
message(STATUS "Compiler is " ${CMAKE_CXX_COMPILER_ID})
message(STATUS "Source dir is " ${CMAKE_SOURCE_DIR})
message(STATUS "Build dir is " ${CMAKE_BIN_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -fno-rtti -std=c++14" )

message(STATUS "CXX Flags: " ${CMAKE_CXX_FLAGS})
message(STATUS "Linker Flags: " ${CMAKE_EXE_LINKER_FLAGS})
set(dir ${CMAKE_CURRENT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}")

set(SOURCE_FILES
                 matrix.cpp
                 mmio.cpp
)

set(HEADER_FILES
                 matrix.h
                 mmio.h
)

add_library(mmmatrixio ${SOURCE_FILES} ${HEADER_FILES})

add_executable(testmatrixio ${SOURCE_FILES} main.cpp ${HEADER_FILES})
add_executable(collectMatrixStats ${SOURCE_FILES} statsCollector.cpp ${HEADER_FILES})

