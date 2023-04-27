cmake_minimum_required(VERSION 3.8)
PROJECT(cvQTcameraGL)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)
find_package( GLUT REQUIRED)
find_package( GLEW   REQUIRED )
find_package( OpenGL REQUIRED)

set(MAIN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(OPENGL_WIDGET_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl_widget.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/opengl_widget.h

)

QT5_WRAP_CPP( OPENGL_WIDGET_MOC_SOURCES
  ${OPENGL_WIDGET_SOURCES}
)

QT5_WRAP_CPP(MAIN_MOCsrcs
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(TEXTFILE_SOURCES
  ${MAIN_SOURCES}
)

add_library(opengl_widget)
target_sources(opengl_widget
  PRIVATE
  ${OPENGL_WIDGET_SOURCES}
  ${OPENGL_WIDGET_MOC_SOURCES}
)
target_include_directories(opengl_widget
 PRIVATE	
	include
)
target_link_libraries(opengl_widget
  ${OpenCV_LIBS} 
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARY}
  GLEW::GLEW
  Qt5::Core
  Qt5::Widgets
)

add_executable(qtopengl_test)
target_sources(qtopengl_test
  PRIVATE
  ${MAIN_MOC_SOURCES}
  ${MAIN_SOURCES}
	)
target_include_directories(qtopengl_test
 PRIVATE
	include
)
target_link_libraries(qtopengl_test
  opengl_widget
)
