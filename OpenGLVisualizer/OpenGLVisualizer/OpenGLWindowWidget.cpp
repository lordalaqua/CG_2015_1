#include "OpenGLWindowWidget.h"

void OpenGLWindowWidget::initializeGL()
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(1);
    model.loadFromFile("C:/Projects/CG_2015_1/OpenGLVisualizer/OpenGLVisualizer/Resources/cow_up.txt");
    findWindingOrder();
    recalculateOriginalPositions();

    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glFrontFace(winding_order);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    resetCamera();
}

void OpenGLWindowWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    updateCamera();
}

void OpenGLWindowWidget::paintGL()
{
    if (reset_camera)
    {
        resetCamera();
        reset_camera = false;
    }
    if (update_order)
    {
        glFrontFace(winding_order);
        update_order = false;
    }
    if (update_camera)
    {
        updateCamera();
        update_camera = false;
    }
    if (update_lighting)
    {
        updateLighting();
        update_lighting = false;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    for (auto triangle : model.triangles)
    {
        for (int i = 2; i >= 0; --i)
        {
            if (lighting_on)
            {
                GLfloat diffuse[] = { color.x, color.y, color.z };
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
            }                
            else
                glColor3f(color.x, color.y, color.z);
            glVertex3f(triangle.vertex[i].x, triangle.vertex[i].y, triangle.vertex[i].z);
            glNormal3f(triangle.normal[i].x, triangle.normal[i].y, triangle.normal[i].z);
        }
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, render_mode);
    glFlush();
}

void OpenGLWindowWidget::recalculateOriginalPositions()
{
    object_center = {
        model.min.x + (model.max.x - model.min.x) / 2,
        model.min.y + (model.max.y - model.min.y) / 2,
        model.min.z + (model.max.z - model.min.z) / 2 };
    float max = std::max(model.max.x - model.min.x, model.max.y - model.min.y);
    camera_original_position = object_center + Vector3f(5, 5, 1.5*(max));
}

void OpenGLWindowWidget::resetCamera()
{
    camera_x = { 1, 0, 0 };
    camera_y = { 0, 1, 0 };
    camera_z = { 0, 0, -1 };
    field_of_view_x = 45;
    field_of_view_y = 45;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(field_of_view_y, (this->width()*field_of_view_x) / (this->height()*field_of_view_y), zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera_look = object_center;
    camera_position = camera_original_position;
    gluLookAt(camera_position.x, camera_position.y, camera_position.z, object_center.x, object_center.y, object_center.z, camera_y.x, camera_y.y, camera_y.z);
    glFlush();
}

void OpenGLWindowWidget::updateCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(field_of_view_y, (this->width()*field_of_view_x)/(this->height()*field_of_view_y), zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera_position.x, camera_position.y, camera_position.z, camera_look.x, camera_look.y, camera_look.z, camera_y.x, camera_y.y, camera_y.z);
    glFlush();
}

void OpenGLWindowWidget::updateLighting()
{
    if(lighting_on)
    {
        glEnable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
        GLfloat light_ambient[] = { 10.0, 10.0, 10.0, 1.0 };
        GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
        GLfloat light_position1[] = { camera_position.x, camera_position.y, camera_position.z, 0.0 };
        GLfloat light_direction1[] = { 0.0, 0.0, 0.0 };
        GLfloat light_exponent[] = { 0.5 };
        glEnable(GL_LIGHT0);
        //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        //glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, light_exponent);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }
}

void OpenGLWindowWidget::findWindingOrder()
{
    // Find winding order
    Vector3f v0 = model.triangles[0].vertex[0];
    Vector3f v1 = model.triangles[0].vertex[1];
    Vector3f v2 = model.triangles[0].vertex[2];
    Vector3f calculated_normal = crossProduct(v1 - v0, v2 - v0).normalize();
    Vector3f face_normal = model.triangles[0].face_normal;
    if (dotProduct(calculated_normal, face_normal) > 0)
        winding_order = GL_CW;
    else
        winding_order = GL_CCW;
}
