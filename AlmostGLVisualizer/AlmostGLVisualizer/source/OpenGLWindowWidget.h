#ifndef OpenGLWindowWidget_h__
#define OpenGLWindowWidget_h__

#include "QTWidgets/QOpenGLWidget"
#include "QOpenGLFunctions"
#include <QTimer>
#include "GL/GLU.h"
#include <cmath>
#include "Model3D.h"
#include "Vector.h"

/*
    OpenGLWindowWidget

    QOpenGLWidget Specialization for exhibition of some simple operations in
    OpenGL such as camera rotation and translation, changes in FOV and near and
    far planes.

    Changes state of OpenGL application based on user interface commands.
*/

class OpenGLWindowWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWindowWidget(QWidget *parent);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

public: // accessors for signals
    GLenum windingOrder() { return winding_order; }
    float fieldOfViewX() { return field_of_view_x; }
    float fieldOfViewY() { return field_of_view_y; }

signals:
    void windingOrderChanged();
    void FOVXchanged();
    void FOVYchanged();

public slots:
    // Color Control
    void setColorR(float R) { color.x = R; }
    void setColorG(float G) { color.y = G; }
    void setColorB(float B) { color.z = B; }

    // Lighting, vertex order and polygon mode(polygons,wireframe,etc)controls
    void toggleLighting(bool is_on) { lighting_on = is_on; update_lighting = true; }
    void setLightingMode(GLenum mode) { lighting_mode = mode; update_lighting = true; }
    void setWindingOrder(GLenum w) { winding_order = w; update_order = true; }
    void switchPolygonMode(GLenum mode) { polygon_mode = mode; }

    // Model load interface
    void loadModel(std::string filename);
    
    //Reset Camera
    void setCameraReset() { reset_camera = true; }

    // Frustum control
    void setZNear(double n) { zNear = n; update_camera = true;}
    void setZFar(double f) { zFar = f; update_camera = true;}
    void setFOVX(double fov) { field_of_view_x = fov; update_camera = true; }
    void setFOVY(double fov) { field_of_view_y = fov; update_camera = true; }

    // Camera Transforms
    void setFixedCenter(bool fixed) { fixed_center = fixed; }
    void translateCameraX(float x);
    void translateCameraY(float y);
    void translateCameraZ(float z);    
    void rotateCameraX(int angle);
    void rotateCameraY(int angle);
    void rotateCameraZ(int angle);

private: // helper methods
    void recalculateOriginalPositions();
    void resetCamera();
    void updateCamera();
    void updateLighting();
    void findWindingOrder();

private:    // Class data members
    // 3D model and object position (without any transform)
    Model3D model;
    Vector3f object_center;

    // Color and lighting variables
    Vector3f color;
    GLenum polygon_mode;
    GLenum winding_order;
    GLenum lighting_mode;
    bool lighting_on;
    bool fixed_center;

    GLdouble field_of_view_x;
    GLdouble field_of_view_y;
    GLdouble zNear;
    GLdouble zFar;

    // Camera variables
    Vector3f camera_original_position;
    Vector3f camera_position;
    Vector3f camera_look;
    Vector3f camera_x;
    Vector3f camera_y;
    Vector3f camera_z;

    // Flags for keeping track of what needs updating
    bool update_camera;
    bool update_order;
    bool reset_camera;
    bool update_lighting;

    // Timer to refresh OpenGL window
    QTimer timer;
};

#endif // OpenGLWindowWidget_h__