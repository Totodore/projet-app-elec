cmake_minimum_required(VERSION 3.0.0)
project(pb1 VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(../../lib/AudioFile)
add_executable(pb1 main.cpp ../../lib/AudioFile/AudioFile.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)