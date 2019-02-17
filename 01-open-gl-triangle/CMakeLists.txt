cmake_minimum_required(VERSION 3.9)
project(04-1-opengl-triangle)

add_library(engine-03-3 SHARED engine.cxx)
target_compile_features(engine-03-3 PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(game-03-3 main.cxx)
target_compile_features(game-03-3 PUBLIC cxx_std_17)

target_link_libraries(engine-03-3
    ${SDL2_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    )

target_link_libraries(game-03-3 engine-03-3)
