cmake_minimum_required(VERSION 3.11)
project("caches")

set(FIFO_SRCS
    source/s3_fifo.cpp; 
)
set(IDL_SRCS
    source/ideal.cpp; 
)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(COMPILE_FLAGS "-Wall")

add_library(FIFOlib STATIC ${FIFO_SRCS})
add_library(IDLlib STATIC ${IDL_SRCS})


target_include_directories(FIFOlib PUBLIC "include")
target_include_directories(IDLlib PUBLIC "include")


add_executable(FIFO source/s3_fifo.cpp)
add_executable(IDL source/ideal.cpp)


target_compile_definitions(FIFO PRIVATE
    $<$<CONFIG:Debug>:Debug=1>
    $<$<CONFIG:Release>:Release=1>
)

target_compile_definitions(IDL PRIVATE
    $<$<CONFIG:Debug>:Debug=1>
    $<$<CONFIG:Release>:Release=1>
)

target_compile_options(FIFO PRIVATE ${COMPILE_FLAGS})
target_compile_options(IDL PRIVATE ${COMPILE_FLAGS})

target_link_libraries(FIFO FIFOlib)
target_link_libraries(IDL IDLlib)



#cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
#cmake --build build
#./build/FIFO