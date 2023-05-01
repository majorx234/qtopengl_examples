#include <QApplication>
#include "opengl_window.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // For OpenGL Graphics
    OpenGlWindow opengl_window;
    opengl_window.resize(800,600);
    opengl_window.show();

    return app.exec();
}

