cmake_minimum_required(VERSION 3.11)
project("caches")

SET(SRCS
source/cache.cpp;
)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


add_library(cachelib STATIC ${SRCS})
target_include_directories(cachelib PUBLIC "include")

add_executable(cache source/cache.cpp)
target_link_libraries(cache cachelib)

#cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
#cmake —build build
#./bin/cache
