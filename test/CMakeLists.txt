cmake_minimum_required(VERSION 3.11)
project("LFU_Cache")

SET(SRCS
source/main.cpp;
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


add_library(testlib STATIC ${SRCS})
target_include_directories(testlib PUBLIC "include")

add_executable(test source/main.cpp)
target_link_libraries(test testlib)

#cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
#cmake —build build
#./build/LFU
