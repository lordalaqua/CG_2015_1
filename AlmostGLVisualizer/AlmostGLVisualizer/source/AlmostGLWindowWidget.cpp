#include "AlmostGLWindowWidget.h"
AlmostGLWindowWidget::AlmostGLWindowWidget(QWidget *parent) : QOpenGLWidget(parent)
, update_camera(false)
, reset_camera(false)
, fixed_center(false)
{
    GL.material = Material({ 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, 2);
    GL.mode = AlmostGL::POINTS;
    GL.order = AlmostGL::CW;
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(300);
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
    glRasterPos2i(0, 0);
    GL.buffer.resize(this->width(),this->height());
    GL.runRasterization();
    glDrawPixels(this->width(), this->height(), GL_RGB, GL_FLOAT, GL.buffer.data.data());
    glFlush();
}



void AlmostGLWindowWidget::updateCamera()
{
    GL.vp.left = 0.0; GL.vp.right = this->width();
    GL.vp.bottom = 0.0; GL.vp.top = this->height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(GL.vp.left, GL.vp.right, GL.vp.bottom, GL.vp.top);
    GL.runVertexPipeline(model);
    glFlush();
}

void AlmostGLWindowWidget::resetCamera()
{
    GL.camera.reset();
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
    GL.camera.setResetLookAt(object_center);
    GL.camera.setResetPosition(object_center + Vector3f{ 0, 0, 1.0f*(max) });
}



void AlmostGLWindowWidget::loadModel(std::string filename)
{
    model.loadFromFile(filename);
    recalculateOriginalPositions();
    findWindingOrder();
    reset_camera = true;
}

void AlmostGLWindowWidget::translateCameraX(float x)
{
    GL.camera.translateU(x,fixed_center);
    update_camera = true;
}

void AlmostGLWindowWidget::translateCameraY(float y)
{
    GL.camera.translateV(y, fixed_center);
    update_camera = true;
}

void AlmostGLWindowWidget::translateCameraZ(float z)
{
    GL.camera.translateN(z, fixed_center);
    update_camera = true;
}

void AlmostGLWindowWidget::findWindingOrder()
{
    // Find winding order
    Vector3f v0 = model.triangles[0].vertex[0];
    Vector3f v1 = model.triangles[0].vertex[1];
    Vector3f v2 = model.triangles[0].vertex[2];
    Vector3f calculated_normal = crossProduct(v1 - v0, v2 - v0).normalize();
    Vector3f face_normal = model.triangles[0].face_normal;
    if (dotProduct(calculated_normal, face_normal) > 0)
        GL.order = AlmostGL::CCW;
    else
        GL.order = AlmostGL::CW;
    emit windingOrderChanged();
}
