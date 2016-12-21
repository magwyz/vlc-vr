cmake_minimum_required(VERSION 3.1)
project(vlc-vr)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

set(SOURCES      src/main.cpp
                 src/gl.cpp
                 src/player.cpp
                 src/userinterface.cpp
                 src/usercontrol.cpp
                 src/button.cpp
                 src/slider.cpp
                 src/playercontroller.cpp
                 src/quaternion.cpp)
set(HEADERS      include/gl.h
                 include/player.h
                 include/userinterface.h
                 include/usercontrol.h
                 include/button.h
                 include/slider.h
                 include/playercontroller.h
                 include/quaternion.h)

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENHMD REQUIRED openhmd)

find_package(LIBVLC REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)

add_executable(vlc-vr ${SOURCES} ${HEADERS})

target_link_libraries(vlc-vr
     ${OPENHMD_LIBRARIES}
     ${LIBVLC_LIBRARY}
     ${OPENGL_LIBRARIES}
     ${GLEW_LIBRARIES}
     ${SDL_LIBRARY}
     ${SDL_IMAGE_LIBRARY})
target_include_directories(vlc-vr PUBLIC
     ${CMAKE_SOURCE_DIR}/include
     ${OPENHMD_INCLUDE_DIRS}
     ${LIBVLC_INCLUDE_DIR}
     ${OPENGL_INCLUDE_DIRS}
     ${GLEW_INCLUDE_DIRS}
     ${SDL_INCLUDE_DIR}
     ${SDL_IMAGE_INCLUDE_DIR})
set_target_properties(vlc-vr PROPERTIES LINK_FLAGS ${OPENHMD_LDFLAGS})

