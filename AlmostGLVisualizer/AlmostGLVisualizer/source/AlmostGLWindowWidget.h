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


    float fieldOfViewX() { return GL.camera.Hfov(); }
    float fieldOfViewY() { return GL.camera.Vfov(); }
    AlmostGL::WindingOrder windingOrder() { return GL.order; }

signals:
    void windingOrderChanged();
    void FOVXchanged();
    void FOVYchanged();
 public slots:
    // Color Control
     void setColorR(float R) { GL.material.ambient[0] = R; update_camera = true; }
     void setColorG(float G) { GL.material.ambient[1] = G; update_camera = true; }
     void setColorB(float B) { GL.material.ambient[2] = B; update_camera = true; }

    // Lighting, vertex order and polygon mode(polygons,wireframe,etc)controls
    void switchPolygonMode(AlmostGL::PolygonMode mode) { GL.mode = mode; }
    void setWindingOrder(AlmostGL::WindingOrder w) { GL.order = w; update_camera = true; }

    // Model load interface
    void loadModel(std::string filename);

    //Reset Camera
    void setCameraReset() { reset_camera = true; }

    // Frustum control
    void setZNear(double n) { GL.camera.Znear(n); update_camera = true; }
    void setZFar(double f) { GL.camera.Zfar(f); update_camera = true; }
    void setFOVX(double fov) { GL.camera.Hfov(fov); update_camera = true; }
    void setFOVY(double fov) { GL.camera.Vfov(fov); update_camera = true; }

    // Camera Transforms
    // Camera Transforms
    void setFixedCenter(bool fixed) { fixed_center = fixed; }
    void translateCameraX(float x);
    void translateCameraY(float y);
    void translateCameraZ(float z);
    void rotateCameraX(int angle) { GL.camera.rotateU(angle); update_camera = true; }
    void rotateCameraY(int angle) { GL.camera.rotateV(angle); update_camera = true; }
    void rotateCameraZ(int angle) { GL.camera.rotateN(angle); update_camera = true; }
private:
    void updateCamera();
    void resetCamera();
    void recalculateOriginalPositions();
    void findWindingOrder();

private: //fields
    // 3D model and object position (without any transform)
    Model3D model;
    Vector3f object_center;
    AlmostGL GL;

    // Flags for keeping track of what needs updating
    bool fixed_center;
    bool update_camera;
    bool reset_camera;

    // Timer to refresh OpenGL window
    QTimer timer;
};

#endif // AlmostGLWindowWidget_h__