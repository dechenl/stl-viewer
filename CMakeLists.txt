cmake_minimum_required(VERSION 3.24)
project(stl-viewer)

include(FetchContent)
FetchContent_Declare(sdl
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.14/SDL3-devel-3.4.14-mingw.zip
    URL_HASH SHA256=5f43d6dfadaf13c63c8f52a9f8a31bcfd497faeb569cae9ba6165126d1ebb960
)
FetchContent_MakeAvailable(sdl)

set(CMAKE_CXX_STANDARD 20)

include(${sdl_SOURCE_DIR}/cmake/SDL3Config.cmake)

add_executable(stl-viewer
    src/main.cc

    src/geometry.cc
    src/geometry/vec2.cc
    src/geometry/vec3.cc
    src/geometry/vec4.cc
    src/geometry/line2d.cc
    src/geometry/line3d.cc
    src/geometry/mat4.cc
    src/geometry/triangle.cc
    src/geometry/quaternion.cc

    src/parser.cc
)
target_include_directories(stl-viewer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(stl-viewer SDL3::SDL3)

add_custom_command(TARGET stl-viewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3-shared>
        $<TARGET_FILE_DIR:stl-viewer>
    COMMENT "Copying SDL3.dll to the executable directory..."
)
