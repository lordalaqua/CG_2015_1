#include "openglvisualizer.h"
#include "QtWidgets/QApplication"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // TODO: command line interface for file loading
    OpenGLVisualizer w;
    w.show();
    return a.exec();
}
