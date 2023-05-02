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
  , needsInitialize(true)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

OpenGlWindow::~OpenGlWindow() {

}

void OpenGlWindow::initialize() {
  // shader part
  m_program = new QOpenGLShaderProgram();
  // shaderProgram = glCreateProgram();
  if (!m_program->addShaderFromSourceFile(
	    QOpenGLShader::Vertex, ":/src/shader/pass_through.vert"))
	{
		qDebug() << "Vertex shader errors :\n" << m_program->log();
	}

	if (!m_program->addShaderFromSourceFile(
	    QOpenGLShader::Fragment, ":/src/shader/uniform_color.frag"))
	{
		qDebug() << "Fragment shader errors :\n" << m_program->log();
	}

	if (!m_program->link())
		qDebug() << "Shader linker errors :\n" << m_program->log();

  float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// create a new buffer for the vertices
	m_vertexBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer); // VBO
	m_vertexBufferObject.create(); // create underlying OpenGL object
	m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw); // must be called before allocate

	m_vertexBufferObject.bind(); // set it active in the context, so that we can write to it
	// int bufSize = sizeof(vertices) = 9 * sizeof(float) = 9*4 = 36 bytes
	m_vertexBufferObject.allocate(vertices, sizeof(vertices) ); // copy data into buffer

  // Initialize the Vertex Array Object (VAO) to record and remember subsequent attribute assocations with
	// generated vertex buffer(s)
	m_vao.create(); // create underlying OpenGL object
  m_vao.bind(); // sets the Vertex Array Object current to the OpenGL context so it monitors attribute assignments
  // now all following enableAttributeArray(), disableAttributeArray() and setAttributeBuffer() calls are
	// "recorded" in the currently bound VBA.

	// Enable attribute array at layout location 0
	m_program->enableAttributeArray(0);
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
  // This maps the data we have set in the VBO to the "position" attribute.
	// 0 - offset - means the "position" data starts at the begin of the memory array
	// 3 - size of each vertex (=vec3) - means that each position-tuple has the size of 3 floats (those are the 3 coordinates,
	//     mind: this is the size of GL_FLOAT, not the size in bytes!

  // Release (unbind) all
	m_vertexBufferObject.release();
	m_vao.release(); // not really necessary, but done for completeness
}

void OpenGlWindow::render() {
  // this function is called for every frame to be rendered on screen
	const qreal retinaScale = devicePixelRatio(); // needed for Macs with retina display
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	// set the background color = clear color
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// use our shader program
	m_program->bind();
	// bind the vertex array object, which in turn binds the vertex buffer object and
	// sets the attribute buffer in the OpenGL context
	m_vao.bind();
	// now draw the triangles:
	// - GL_TRIANGLES - draw individual triangles
	// - 0 index of first triangle to draw
	// - 3 number of vertices to process
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// finally release VAO again (not really necessary, just for completeness)
	m_vao.release();
}

void OpenGlWindow::renderNow() {
  if (!isExposed())
    return;
  // initialize on first call
	if (m_context == nullptr) {
    m_context = new QOpenGLContext(this);
    m_context->setFormat(requestedFormat());
    m_context->create();
	}

  m_context->makeCurrent(this);

  if (needsInitialize) {
    initializeOpenGLFunctions();
    initialize(); // call user code
    needsInitialize = false;
  }
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
