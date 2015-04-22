#include "AlmostGLWindowWidget.h"

AlmostGLWindowWidget::AlmostGLWindowWidget(QWidget *parent) : QOpenGLWidget(parent)
, color(1.0, 0.0, 0.0)
, polygon_mode(GL_FILL)
, update_camera(false)
, reset_camera(false)
, viewport_left(0.0)
, viewport_right(0.0)
, viewport_bottom(0.0)
, viewport_top(0.0)
, fixed_center(false)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(100);
}

void AlmostGLWindowWidget::initializeGL()
{
    loadModel("C:/Projects/CG_2015_1/OpenGLVisualizer/OpenGLVisualizer/Resources/cow_up.txt");
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1);
    resetCamera();
}

void AlmostGLWindowWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    updateCamera();
}

void AlmostGLWindowWidget::paintGL()
{
    if (reset_camera)
    {
        resetCamera();
        reset_camera = false;
    }
    if (update_camera)
    {
        updateCamera();
        update_camera = false;
    }
    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(color.x, color.y, color.z);
    for (const auto& triangle : triangles)
    {
        for (int i = 0; i < 3; ++i)
        {
            glVertex2i(triangle.vertex[i][0], triangle.vertex[i][1]);
        }
    }
    glEnd();
    glFlush();
}



void AlmostGLWindowWidget::updateCamera()
{
    viewport_left = 0.0; viewport_right = this->width();
    viewport_bottom = 0.0; viewport_top = this->height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();    
    gluOrtho2D(viewport_left,viewport_right,viewport_bottom,viewport_top);
    triangles = AlmostGL::runPipeline(model, camera, viewport_left, viewport_right, viewport_top, viewport_bottom);
    glFlush();
}

void AlmostGLWindowWidget::resetCamera()
{
    camera.reset();
    emit FOVXchanged();
    emit FOVYchanged();
    updateCamera();
}

void AlmostGLWindowWidget::recalculateOriginalPositions()
{
    object_center = {
        model.min.x + (model.max.x - model.min.x) / 2,
        model.min.y + (model.max.y - model.min.y) / 2,
        model.min.z + (model.max.z - model.min.z) / 2 };
    float max = std::max(model.max.x - model.min.x, model.max.y - model.min.y);
    camera.setResetLookAt(object_center);
    camera.setResetPosition(object_center + Vector3f(0, 0, 1.5*(max)));
}



void AlmostGLWindowWidget::loadModel(std::string filename)
{
    model.loadFromFile(filename);
    recalculateOriginalPositions();
    reset_camera = true;
}

void AlmostGLWindowWidget::translateCameraX(float x)
{
    camera.translateU(x,fixed_center);
    update_camera = true;
}

void AlmostGLWindowWidget::translateCameraY(float y)
{
    camera.translateV(y,fixed_center);
    update_camera = true;
}

void AlmostGLWindowWidget::translateCameraZ(float z)
{
    camera.translateN(z,fixed_center);
    update_camera = true;
}
