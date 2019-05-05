cmake_minimum_required(VERSION 3.9)
project(03-texture)

add_library(engine SHARED engine_impl.cpp)
target_compile_features(engine PUBLIC cxx_std_17)

add_library(event SHARED event.cpp)
target_compile_features(event PUBLIC cxx_std_17)

add_library(shader SHARED shader.cpp)
target_compile_features(shader PUBLIC cxx_std_17)

add_library(texture SHARED texture.cpp)
target_compile_features(texture PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(game-03-3 main.cxx)
target_compile_features(game-03-3 PUBLIC cxx_std_17)

target_link_libraries(engine
    ${SDL2_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    )

target_link_libraries(shader
    ${OPENGL_gl_LIBRARY}
    )

target_link_libraries(event
    ${SDL2_LIBRARIES}
    )

target_link_libraries(game-03-3 engine event shader texture)
