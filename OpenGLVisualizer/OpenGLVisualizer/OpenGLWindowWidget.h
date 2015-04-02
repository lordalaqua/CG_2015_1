#pragma once
#include "QTWidgets/QOpenGLWidget"
#include "QOpenGLFunctions"
#include <QTimer>
#include "GL/GLU.h"

#include <cmath>

#include "Model3D.h"
#include "Vector3D.h"


class OpenGLWindowWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWindowWidget(QWidget *parent) : QOpenGLWidget(parent) {};
    ~OpenGLWindowWidget() {};
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
private:
    void recalculateOriginalPositions();
    void resetCamera();
    void updateCamera();
    void updateLighting();
    void findWindingOrder();
private:
    Vector3f color = { 1.0, 0.0, 0.0 };
    GLenum render_mode = GL_FILL;
    GLenum winding_order = GL_CCW;
    bool update_camera = false;
    bool update_order = false;
    bool reset_camera = false;
    bool update_lighting = false;
    bool lighting_on = false;
    bool fixed_center = false;
    Model3D model;
    QTimer timer;
    GLdouble field_of_view_x = 45;
    GLdouble field_of_view_y = 45;
    GLdouble zNear = 0.1;
    GLdouble zFar = 10000.0;

    Vector3f object_center;
    Vector3f camera_original_position;
    Vector3f camera_position;
    Vector3f camera_look;
    Vector3f camera_x = { 1, 0, 0 };
    Vector3f camera_y = { 0, 1, 0 };
    Vector3f camera_z = { 0, 0, -1 };

public slots:
    void switchRenderMode(GLenum mode) { render_mode = mode; }
    void setColorR(float R) { color.x = R; }
    void setColorG(float G) { color.y = G; }
    void setColorB(float B) { color.z = B; }

    void toggleLighting(bool is_on) { lighting_on = is_on; update_lighting = true;}
    void setWindingOrder(GLenum w) { winding_order = w; update_order = true; }
    void setZNear(double n) { zNear = n; update_camera = true;}
    void setZFar(double f) { zFar = f; update_camera = true;}
    void setFOVX(double fov) { field_of_view_x = fov; update_camera = true; }
    void setFOVY(double fov) { field_of_view_y = fov; update_camera = true; }
    void setCameraReset() { reset_camera = true; }
    void setFixedCenter(bool fixed) { fixed_center = fixed; }
    void loadModel(std::string filename)
    {
        model.loadFromFile(filename);
        findWindingOrder();
        recalculateOriginalPositions();
        update_order = true;
        reset_camera = true;
    }
    void translateCameraX(float x) 
    {
        if (fixed_center)
        {
            camera_position = camera_position + x*camera_x;
            camera_z = (camera_look - camera_position).normalize();
            camera_x = crossProduct(camera_y, camera_z).normalize();
        }
        else
        {
            camera_position = camera_position + x*camera_x;
            camera_look = camera_look + x*camera_x;
        }
        update_camera = true;
    }
    void translateCameraY(float y) 
    {        
        if (fixed_center)
        {
            camera_position = camera_position + y*camera_y;
            camera_z = (camera_look - camera_position).normalize();
            camera_y = crossProduct(camera_x, camera_z).normalize();
        }
        else
        {
            camera_position = camera_position + y*camera_y;
            camera_look = camera_look + y*camera_y;
        }
        update_camera = true; 
    }
    void translateCameraZ(float z)
    {
        if (fixed_center)
        {
            camera_position = camera_original_position + z*camera_z;
            camera_look = object_center + z*camera_z;
        }
        else
        {
            camera_position = camera_position + z*camera_z;
            camera_look = camera_look + z*camera_z;
        }
            update_camera = true; 
    }
    
    void rotateCameraX(int angle) 
    { 
        camera_y = rotate(camera_y, camera_x, angle);
        camera_look = rotate(camera_look-camera_position, camera_x, angle)+camera_position;
        camera_z = (camera_look - camera_position).normalize();
        update_camera = true;
    }
    void rotateCameraY(int angle) 
    {
        camera_x = rotate(camera_x, camera_y, angle);
        camera_look = rotate(camera_look - camera_position, camera_y, angle) + camera_position;
        camera_z = (camera_look - camera_position).normalize();
        update_camera = true;
    }
    void rotateCameraZ(int angle) 
    {
        camera_y = rotate(camera_y, camera_z, angle);
        camera_x = rotate(camera_x, camera_z, angle);
        update_camera = true;
    }
// signals:
//     void FOVchanged()
};

