#include "OpenGLWindowWidget.h"
#include "QGLWidget"

/* Initializes Class members to starting values */
OpenGLWindowWidget::OpenGLWindowWidget(QWidget *parent) 
: QOpenGLWidget(parent)
, field_of_view_x(60)
, field_of_view_y(60)
, zNear(1)
, zFar(10000.0)
, camera_x({ 1, 0, 0 })
, camera_y({ 0, 1, 0 })
, camera_z({ 0, 0,-1 })
, polygon_mode(GL_FILL)
, winding_order(GL_CCW)
, lighting_on(false)
, lighting_mode(GL_SMOOTH)
, fixed_center(false)
, update_camera(false)
, update_order(false)
, reset_camera(false)
, update_lighting(false)
, texture(QOpenGLTexture::Target2D)
, texture_on(false)
, texture_loaded(false)
, update_texture(true)
, tex_mode(GL_DECAL)
, min_filter(QOpenGLTexture::Linear)
, mag_filter(QOpenGLTexture::Linear)
{
    ambient_light = { 0.5f, 0.5f, 0.5f };
    light_color = { 1.f, 1.f, 1.f };
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(200);
}

//=========================== Main OpenGL Loop Functions =======================
/* Initialize OpenGL related functions */
void OpenGLWindowWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1);
    glFrontFace(winding_order);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    resetCamera();
    loadModel("C:/Projects/CG_2015_1/AlmostGLVisualizer/AlmostGLVisualizer/Resources/cube_text.in");
    model.material = Material({ 1.f, 0.4f, 0.f }, { .5f, .5f, .5f }, { .5f, .5f, .5f }, 5);
}

/* Handle viewport resizing */
void OpenGLWindowWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    updateCamera();
}

/* Drawing function executed for every frame */
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
    if (texture_on)//update_texture)
    {
        if (!texture_loaded)
        {
            loadTexture("C:/Projects/CG_2015_1/AlmostGLVisualizer/AlmostGLVisualizer/Resources/checker_8x8.jpg");
            //texture_loaded = true;
        }
        //update_texture = false;
    }

    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : model.triangles)
    {
        for (int i = 0; i < 3; ++i)
        {   
            if (texture_on)
            {
                glTexCoord2f(triangle.texture[i].x, triangle.texture[i].y);
            }
            if (lighting_on)
            {
                GLfloat ambient[] = { model.material.ambient[0], model.material.ambient[1], model.material.ambient[2] };
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
                GLfloat diffuse[] = { model.material.diffuse.x, model.material.diffuse.y, model.material.diffuse.z };
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
                GLfloat specular[] = { model.material.specular.x, model.material.specular.y, model.material.specular.z };
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
                GLfloat shine = model.material.shine;
                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, shine);
            }
            else
            {                    
                glColor3f(model.material.ambient[0], model.material.ambient[1], model.material.ambient[2]);
            }
            glNormal3f(triangle.normal[i].x, triangle.normal[i].y, triangle.normal[i].z);
            glVertex3f(triangle.vertex[i].x, triangle.vertex[i].y, triangle.vertex[i].z);
        }
    }
    glEnd();
    glFlush();
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
        glEnable(GL_LIGHTING);
        glShadeModel(lighting_mode);
        GLfloat light_ambient[] = {ambient_light[0],ambient_light[1],ambient_light[2]};
        GLfloat color[] = { light_color[0],light_color[1],light_color[2]};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
        glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
        glLightfv(GL_LIGHT0, GL_SPECULAR, color);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.f);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }
}

//========================= Getters for GUI elements ===========================
GLenum OpenGLWindowWidget::windingOrder()
{
    return winding_order;
}

float OpenGLWindowWidget::fieldOfViewY()
{
    return field_of_view_y;
}

float OpenGLWindowWidget::fieldOfViewX()
{
    return field_of_view_x;
}

//===================== Slots to connect GUI element actions ===================

//       =========== Loading model and calculating model info =============
void OpenGLWindowWidget::loadModel(std::string filename)
{
    model.loadFromFile(filename);
    findWindingOrder();
    recalculateOriginalPositions();
    update_order = true;
    reset_camera = true;
}

void OpenGLWindowWidget::loadTexture(std::string filename)
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, tex_mode);
    texture.setMinificationFilter(min_filter);
    texture.setMagnificationFilter(mag_filter);
    // Load texture from Image, and generate MipMaps (default)
    QImage img(filename.c_str());
    texture.setData(img.mirrored());
    texture.bind();
    texture.wrapMode(QOpenGLTexture::DirectionS);
    texture.wrapMode(QOpenGLTexture::DirectionT);
}

void OpenGLWindowWidget::recalculateOriginalPositions()
{
    object_center = {
        model.min.x + (model.max.x - model.min.x) / 2,
        model.min.y + (model.max.y - model.min.y) / 2,
        model.min.z + (model.max.z - model.min.z) / 2 };
    float max = std::max(model.max.x - model.min.x, model.max.y - model.min.y);
    camera_original_position = object_center + Vector3f{ 0, 0, 1.4f*(max) };
    light_position[0] = camera_original_position.z;
    light_position[1] = camera_original_position.y;
    light_position[2] = camera_original_position.x;
    light_position[3] = 1;
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

//       ================ Camera translation and rotation ==================

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

void OpenGLWindowWidget::setFixedCenter(bool fixed)
{
    fixed_center = fixed;
}

void OpenGLWindowWidget::setFOVY(double fov)
{
    field_of_view_y = fov; update_camera = true;
}

void OpenGLWindowWidget::setFOVX(double fov)
{
    field_of_view_x = fov; update_camera = true;
}

void OpenGLWindowWidget::setZFar(double f)
{
    zFar = f; update_camera = true;
}

void OpenGLWindowWidget::setZNear(double n)
{
    zNear = n; update_camera = true;
}

void OpenGLWindowWidget::setCameraReset()
{
    reset_camera = true;
}

//       ======== Rendering mode and lighting properties controls ========

void OpenGLWindowWidget::switchPolygonMode(GLenum mode)
{
    polygon_mode = mode;
}

void OpenGLWindowWidget::setWindingOrder(GLenum w)
{
    winding_order = w; update_order = true;
}

void OpenGLWindowWidget::setLightingMode(GLenum mode)
{
    lighting_mode = mode; update_lighting = true;
}

void OpenGLWindowWidget::toggleLighting(bool is_on)
{
    lighting_on = is_on; update_lighting = true;
}



