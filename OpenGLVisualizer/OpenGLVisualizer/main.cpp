#include "openglvisualizer.h"
#include "QtWidgets/QApplication"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenGLVisualizer w;
    w.show();
    return a.exec();
}
