cmake_minimum_required(VERSION 3.6)
project(SDL2_Playground)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SOURCE_FILES main.cpp)
add_executable(SDL2_Playground ${SOURCE_FILES})

target_link_libraries(SDL2_Playground SDL2main SDL2)