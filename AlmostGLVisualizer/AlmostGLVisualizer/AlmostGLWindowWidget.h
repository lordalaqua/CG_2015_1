#ifndef AlmostGLWindowWidget_h__
#define AlmostGLWindowWidget_h__

#include "QTWidgets/QOpenGLWidget"
#include "QOpenGLFunctions"
#include <QTimer>
#include "GL/GLU.h"

class AlmostGLWindowWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    AlmostGLWindowWidget();
    ~AlmostGLWindowWidget();
    void initializeGL() {};
    void resizeGL(int width, int height) {};
    void paintGL() {};
};

#endif // AlmostGLWindowWidget_h__