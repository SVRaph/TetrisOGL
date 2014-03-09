cmake_minimum_required(VERSION 2.8)

project(TetrisOGL)
add_executable(tetris constants.cpp gl2d.cpp tetrominos.cpp terrain.cpp players.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/api/lib/)

FIND_LIBRARY(FMOD NAMES libfmodex.so PATHS ${CMAKE_CURRENT_BINARY_DIR}/api/lib)

target_link_libraries(tetris ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FMOD})
