cmake_minimum_required(VERSION 2.6.0)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenCV REQUIRED)
find_package(Qt4 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(LibV4LConvert REQUIRED)

link_directories(${GLUT_LIBRARY_DIRS}
                 ${OpenGL_LIBRARY_DIRS}
                 ${LIBV4LCONVERT_LIBRARY})

add_definitions(${OpenGL_DEFINITIONS})
add_definitions(${GLUT_DEFINITIONS})

include(${QT_USE_FILE})

QT4_WRAP_UI(UI_TESCOCAM_H 
            tescocam.ui)

qt4_wrap_cpp(
        tescocam_moc_SRCS
        tescocam.h
        )

include_directories(${OpenCV_INCLUDE_DIRS}
                    ${GLUT_INCLUDE_DIRS}
                    ${OpenGL_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_PATH}
                    ${LIBV4LCONVERT_INCLUDE_DIR})

configure_file(checkerboard.png ${CMAKE_CURRENT_BINARY_DIR}/checkerboard.png COPYONLY)
configure_file(intrinsics.xml ${CMAKE_CURRENT_BINARY_DIR}/intrinsics.xml COPYONLY)
configure_file(distortion.xml ${CMAKE_CURRENT_BINARY_DIR}/distortion.xml COPYONLY)

add_definitions(-Wall)

set(CMAKE_CXX_FLAGS "-g -O3 -msse2 -msse3")

FILE(GLOB srcs *.cpp)
source_group("Source Files" FILES ${srcs} )

add_executable(PAC7302
               ${UI_TESCOCAM_H}
               ${tescocam_moc_SRCS}
               ${srcs}
)

target_link_libraries(PAC7302
                      ${OpenCV_LIBS}
                      ${QT_LIBRARIES}
                      ${QT_QTOPENGL_LIBRARY}
                      ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

