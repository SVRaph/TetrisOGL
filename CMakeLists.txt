cmake_minimum_required(VERSION 2.8)

project(TetrisOGL)


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Boost 1.40.0 REQUIRED COMPONENTS thread system)
find_package (Threads)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../fmod_api/inc/)

find_library(FMOD NAMES libfmodex.so PATHS ${CMAKE_CURRENT_BINARY_DIR}/../fmod_api/lib)


#stand alone
add_executable(tetris constants.cpp gl2d.cpp tetrominos.cpp terrain.cpp players.cpp)
target_link_libraries(tetris ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FMOD})
# Client - serveur
add_executable(serveur tetris_serveur.cpp)
target_link_libraries(serveur ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
add_executable(client  tetris_client.cpp constants.cpp tetrominos.cpp terrain.cpp players.cpp)
target_link_libraries(client ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FMOD} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY})


## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++0x -lpthread")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-lpthread")  ## Strip binary
endif()

