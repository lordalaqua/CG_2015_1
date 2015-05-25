#include "OpenGLWindowWidget.h"

OpenGLWindowWidget::OpenGLWindowWidget(QWidget *parent) 
: QOpenGLWidget(parent)
, color({ 1.0, 0.0, 0.0 })
, polygon_mode(GL_POINT)
, winding_order (GL_CCW)
, update_camera(false)
, update_order(false)
, reset_camera(false)
, update_lighting(false)
, lighting_on(false)
, lighting_mode(GL_SMOOTH)
, fixed_center(false)
, field_of_view_x(60)
, field_of_view_y(60)
, zNear(1)
, zFar(10000.0)
, camera_x({ 1, 0, 0 })
, camera_y({ 0, 1, 0 })
, camera_z({ 0, 0, -1 })
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(300);
}

void OpenGLWindowWidget::initializeGL()
{
    loadModel("C:/Projects/CG_2015_1/OpenGLVisualizer/OpenGLVisualizer/Resources/cow_up.txt");
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1);
    glFrontFace(winding_order);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
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
    if (update_camera)
    {
        updateCamera();
        update_camera = false;
    }
    if (update_order)
    {
        glFrontFace(winding_order);
        update_order = false;
    }
    if (update_lighting)
    {
        updateLighting();
        update_lighting = false;
    }

    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : model.triangles)
    {
        for (int i = 0; i < 3; ++i)
        {            
            if (lighting_on)
            {
                auto v_specular = model.materials[triangle.material_index[i]].specular;
                GLfloat specular[] = { v_specular.x, v_specular.y, v_specular.z };
                //auto v_shine = model.materials[triangle.material_index[i]].shine;
                GLfloat shine[] = { 12 };
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
            }            
            glColor3f(color.x, color.y, color.z);
            glNormal3f(triangle.normal[i].x, triangle.normal[i].y, triangle.normal[i].z);
            glVertex3f(triangle.vertex[i].x, triangle.vertex[i].y, triangle.vertex[i].z);
        }
    }
    glEnd();
    glFlush();
}

void OpenGLWindowWidget::recalculateOriginalPositions()
{
    object_center = {
        model.min.x + (model.max.x - model.min.x) / 2,
        model.min.y + (model.max.y - model.min.y) / 2,
        model.min.z + (model.max.z - model.min.z) / 2 };
    float max = std::max(model.max.x - model.min.x, model.max.y - model.min.y);
    camera_original_position = object_center + Vector3f{ 0, 0, 1.0f*(max) };
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
        winding_order = GL_CCW;
    else
        winding_order = GL_CW;
    emit windingOrderChanged();
}

void OpenGLWindowWidget::resetCamera()
{
    camera_x = { 1, 0, 0 };
    camera_y = { 0, 1, 0 };
    camera_z = { 0, 0, 1 };
    field_of_view_x = 60;
    field_of_view_y = 60;
    zNear = 1;
    zFar = 10000;
    emit FOVXchanged();
    emit FOVYchanged();    
    camera_look = object_center;
    camera_position = camera_original_position;
    updateCamera();
}

void OpenGLWindowWidget::updateCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(field_of_view_y, (field_of_view_x)/(field_of_view_y), zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera_position.x, camera_position.y, camera_position.z, camera_look.x, camera_look.y, camera_look.z, camera_y.x, camera_y.y, camera_y.z);
    glFlush();
}

void OpenGLWindowWidget::updateLighting()
{
    if(lighting_on)
    {
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        
        glShadeModel(lighting_mode);
        GLfloat light_ambient[] = { 1.f, 0.f, 1.f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
        GLfloat light_diffuse[] = { 1.0, 1.0, 1.0 };
        GLfloat light_specular[] = { 1.0, 1.0, 1.0 };
        GLfloat light_position1[] = { camera_position.x, camera_position.y, camera_position.z};
        GLfloat light_direction1[] = { camera_z.x, camera_z.y, camera_z.z };
        GLfloat light_exponent[] = { 128 };
        GLfloat light_cutoff[] = { 180 };
        //glEnable(GL_LIGHT0);
        //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        //glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction1);
        //glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, light_exponent);
        //glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, light_cutoff);
        //glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }
}

// SLOTS

void OpenGLWindowWidget::loadModel(std::string filename)
{
    model.loadFromFile(filename);
    findWindingOrder();
    recalculateOriginalPositions();
    update_order = true;
    reset_camera = true;
}

void OpenGLWindowWidget::translateCameraX(float x)
{
    if (fixed_center)
    {
        camera_position = camera_position + x*camera_x;
        camera_z = -(camera_look - camera_position).normalize();
        camera_x = crossProduct(camera_y, camera_z).normalize();
    }
    else
    {
        camera_position = camera_position + x*camera_x;
        camera_look = camera_look + x*camera_x;
    }
    update_camera = true;
}

void OpenGLWindowWidget::translateCameraY(float y)
{
    if (fixed_center)
    {
        camera_position = camera_position + y*camera_y;
        camera_z = -(camera_look - camera_position).normalize();
        camera_y = crossProduct(camera_z, camera_x).normalize();
    }
    else
    {
        camera_position = camera_position + y*camera_y;
        camera_look = camera_look + y*camera_y;
    }
    update_camera = true;
}

void OpenGLWindowWidget::translateCameraZ(float z)
{
    camera_position = camera_position + z*camera_z;
    camera_look = camera_look + z*camera_z;
    update_camera = true;
}

void OpenGLWindowWidget::rotateCameraX(int angle)
{
    camera_y = rotate(camera_y, camera_x, angle);
    camera_look = rotate(camera_look - camera_position, camera_x, angle) + camera_position;
    camera_z = (camera_look - camera_position).normalize();
    update_camera = true;
}

void OpenGLWindowWidget::rotateCameraY(int angle)
{
    camera_x = rotate(camera_x, camera_y, angle);
    camera_look = rotate(camera_look - camera_position, camera_y, angle) + camera_position;
    camera_z = (camera_look - camera_position).normalize();
    update_camera = true;
}

void OpenGLWindowWidget::rotateCameraZ(int angle)
{
    camera_y = rotate(camera_y, camera_z, angle);
    camera_x = rotate(camera_x, camera_z, angle);
    update_camera = true;
}


