#ifndef __APPLE__
 //   #include <GL/glu.h>
    #include <GL/glew.h>
    #include <GL/glut.h>
#else
    #include <glu.h>
#endif

#include "opengl_window.hpp"
#include <qcoreevent.h>
#include <qwindow.h>

#include <iostream>
#include <QEvent>
#include <QTimer>
#include <QString>

// forward declaration debug functions
#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(char *file, int line); 
void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);

OpenGlWindow::OpenGlWindow(QWindow *parent)
  : QWindow(parent)
  , m_context(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

OpenGlWindow::~OpenGlWindow() {

}

void OpenGlWindow::initialize() {
  if (!isExposed())
    return;

  // initialize on first call
	if (m_context == nullptr) {
    m_context = new QOpenGLContext(this);
    m_context->setFormat(requestedFormat());
    m_context->create();
	}

  m_context->makeCurrent(this);

  initializeOpenGLFunctions();
  initialize(); // call user code

  // shader part
  m_program = new QOpenGLShaderProgram();
  // shaderProgram = glCreateProgram();
  if (!m_program->addShaderFromSourceFile(
	    QOpenGLShader::Vertex, ":/shaders/pass_through.vert"))
	{
		qDebug() << "Vertex shader errors :\n" << m_program->log();
	}

	if (!m_program->addShaderFromSourceFile(
	    QOpenGLShader::Fragment, ":/shaders/uniform_color.frag"))
	{
		qDebug() << "Fragment shader errors :\n" << m_program->log();
	}

	if (!m_program->link())
		qDebug() << "Shader linker errors :\n" << m_program->log();  
}

void OpenGlWindow::render() {
}

void OpenGlWindow::renderNow() {
  //  m_context->makeCurrent(this);
  render();
  m_context->swapBuffers(this);
}

void OpenGlWindow::renderLater() {
}


bool OpenGlWindow::event(QEvent *event) {
    switch (event->type()) {
      case QEvent::UpdateRequest:
        renderNow();
        return true;
      default:
        return QWindow::event(event);
    }
}


void OpenGlWindow::exposeEvent(QExposeEvent * /*event*/) {
    renderNow(); // simply redirect call to renderNow()
}
