#ifndef OPENGL_WINDOW_HPP_
#define OPENGL_WINDOW_HPP_
#include <qevent.h>
#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <qopenglcontext.h>


class OpenGlWindow : public QWindow, protected QOpenGLFunctions
{
  Q_OBJECT
 public:
  explicit OpenGlWindow(QWindow* parent = 0);
  virtual ~OpenGlWindow();

  /* OpenGL initialization, viewport resizing, and painting */

  virtual void initialize();
  virtual void render();
 public slots:
  void renderLater();
  void renderNow();
   
 protected:
private:
  bool event(QEvent *event) override;
  void exposeEvent(QExposeEvent*event) override;

  QOpenGLContext *m_context;

  QOpenGLVertexArrayObject m_vao;
  QOpenGLBuffer	m_vertexBufferObject;
  QOpenGLShaderProgram *m_program;
};

#endif // OPENGL_WINDOW_HPP_
