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
    GLenum windingOrder();
    float fieldOfViewX();
    float fieldOfViewY();

signals:
    void windingOrderChanged();
    void FOVXchanged();
    void FOVYchanged();

public slots:
    // Color Control
    void setColorR(float R);
    void setColorG(float G);
    void setColorB(float B);

    // Lighting, vertex order and polygon mode(polygons,wireframe,etc)controls
    void toggleLighting(bool is_on);
    void setLightingMode(GLenum mode);
    void setWindingOrder(GLenum w);
    void switchPolygonMode(GLenum mode);

    // Model load interface
    void loadModel(std::string filename);
    
    //Reset Camera
    void setCameraReset();

    // Frustum control
    void setZNear(double n);
    void setZFar(double f);
    void setFOVX(double fov);
    void setFOVY(double fov);

    // Camera Transforms
    void setFixedCenter(bool fixed);
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
    GLenum polygon_mode;
    GLenum winding_order;
    GLenum lighting_mode;
    bool lighting_on;

    Vector3f ambient_light;
    Vector3f light_color;

    // Translation control variable
    bool fixed_center;

    // Camera and viewport variables
    GLdouble field_of_view_x;
    GLdouble field_of_view_y;
    GLdouble zNear;
    GLdouble zFar;

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