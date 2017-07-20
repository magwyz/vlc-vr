cmake_minimum_required(VERSION 3.1)
project(vlc-vr)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

set(SOURCES      src/main.cpp
                 src/gl.cpp
                 src/player.cpp
                 src/playercontroller.cpp
                 src/quaternion.cpp)
set(HEADERS      include/gl.h
                 include/player.h
                 include/userinterface.h
                 include/usercontrol.h
                 include/button.h
                 include/slider.h
                 include/label.h
                 include/playercontroller.h
                 include/quaternion.h
                 include/screen.h)

find_package(OpenHMD REQUIRED)
find_package(LIBVLC REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(vlc-vr ${SOURCES} ${HEADERS})

target_link_libraries(vlc-vr
     ${OpenHMD_LIBRARIES}
     ${LIBVLC_LIBRARY}
     ${OPENGL_LIBRARIES}
     ${GLEW_LIBRARIES}
     ${SDL2_LIBRARY}
     ${SDL2_IMAGE_LIBRARY}
     ${SDL2_TTF_LIBRARY})
target_include_directories(vlc-vr PUBLIC
     ${CMAKE_SOURCE_DIR}/include
     ${OpenHMD_INCLUDES}
     ${LIBVLC_INCLUDE_DIR}
     ${OPENGL_INCLUDE_DIRS}
     ${GLEW_INCLUDE_DIRS}
     ${SDL2_INCLUDE_DIR}
     ${SDL2_IMAGE_INCLUDE_DIR}
     ${SDL2_TTF_INCLUDE_DIR})


