#ifndef AlmostGLWindowWidget_h__
#define AlmostGLWindowWidget_h__

#include "QTWidgets/QOpenGLWidget"
#include "QOpenGLFunctions"
#include <QTimer>
#include <GL/GLU.h>
#include "Model3D.h"
#include "Camera.h"
#include "AlmostGLPipeline.h"

/*
    AlmostGLWindowWidget

    Run the AlmostGL pipeline in a loaded model.
    QT Widget to interface with AlmostGL rendering pipeline.
*/
class AlmostGLWindowWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    AlmostGLWindowWidget(QWidget *parent);
    void initializeGL();
    void resizeGL(int width, int height);;
    void paintGL();;


    float fieldOfViewX() { return camera.Hfov(); }
    float fieldOfViewY() { return camera.Vfov(); }

signals:
    void FOVXchanged();
    void FOVYchanged();
 public slots:
    // Color Control
    void setColorR(float R) { color.x = R; }
    void setColorG(float G) { color.y = G; }
    void setColorB(float B) { color.z = B; }

    // Lighting, vertex order and polygon mode(polygons,wireframe,etc)controls
    void switchPolygonMode(GLenum mode) { polygon_mode = mode; }

    // Model load interface
    void loadModel(std::string filename);

    //Reset Camera
    void setCameraReset() { reset_camera = true; }

    // Frustum control
    void setZNear(double n) { camera.Znear(n); update_camera = true; }
    void setZFar(double f) { camera.Zfar(f); update_camera = true; }
    void setFOVX(double fov) { camera.Hfov(fov); update_camera = true; }
    void setFOVY(double fov) { camera.Vfov(fov); update_camera = true; }

    // Camera Transforms
    // Camera Transforms
    void setFixedCenter(bool fixed) { fixed_center = fixed; }
    void translateCameraX(float x);
    void translateCameraY(float y);
    void translateCameraZ(float z);
    void rotateCameraX(int angle) { camera.rotateU(angle); update_camera = true; }
    void rotateCameraY(int angle) { camera.rotateV(angle); update_camera = true; }
    void rotateCameraZ(int angle) { camera.rotateN(angle); update_camera = true; }
private:
    void updateCamera();
    void resetCamera();
    void recalculateOriginalPositions();

private: //fields
    // 3D model and object position (without any transform)
    Model3D model;
    Vector3f object_center;
    std::vector<AlmostGL::Triangle4D> triangles;

    // Virtual camera and original position based on model load.
    Camera camera;
    Vector3f camera_original_position;
    float viewport_left, viewport_right, viewport_bottom, viewport_top;

    // Color and lighting variables
    Vector3f color;
    GLenum polygon_mode;

    // Flags for keeping track of what needs updating
    bool fixed_center;
    bool update_camera;
    bool reset_camera;

    // Timer to refresh OpenGL window
    QTimer timer;
};

#endif // AlmostGLWindowWidget_h__