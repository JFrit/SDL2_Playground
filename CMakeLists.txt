cmake_minimum_required(VERSION 3.6)
project(SDL2_Playground)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_compile_definitions("_OPENGL")

set(SOURCE_FILES src/main.cpp src/glad.c include/sp/Window.h src/Window.cpp)
add_executable(SDL2_Playground ${SOURCE_FILES})

target_link_libraries(SDL2_Playground SDL2main SDL2)