cmake_minimum_required(VERSION 3.15)
project(spacegame C)

set(CMAKE_FIND_FRAMEWORK LAST)

if(APPLE)
    set(OPENAL_INCLUDE_DIR "/usr/local/Cellar/openal-soft/1.20.1/include/AL")
    set(OPENAL_LIBRARY "/usr/local/Cellar/openal-soft/1.20.1/lib/libopenal.dylib")
    message("setting openaldir manually")
else()
    include(FindOpenAL)
endif()

include(FindOpenGL)
include(FindGLEW)

find_package(GLFW3 REQUIRED)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_SOURCE_DIR src)

message("GOD ISEEE DEAD")

add_executable(spacegame src/main.c src/memory.c src/error.c src/glfw_window.c src/shader.c src/file.c src/audio.c src/str_util.c src/list.c src/obj.c src/wav.c)

target_include_directories(spacegame PUBLIC ${OPENAL_INCLUDE_DIR})
target_include_directories(spacegame PUBLIC ${GLEW_INCLUDE_DIR})

target_link_libraries(spacegame LINK_PUBLIC GLEW::glew)
target_link_libraries(spacegame LINK_PUBLIC ${OPENAL_LIBRARY})
target_link_libraries(spacegame LINK_PUBLIC glfw)