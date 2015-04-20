#include "almostglvisualizer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AlmostGLVisualizer w;
    w.show();
    return a.exec();
}
