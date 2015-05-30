#ifndef AlmostGLWindowWidget_h__
#define AlmostGLWindowWidget_h__

#include "QTWidgets/QOpenGLWidget"
#include "QOpenGLFunctions"
#include <QTimer>
#include <GL/GLU.h>
#include "Model3D.h"
#include "Camera.h"
#include "Pipeline.h"
#include "AlmostGLParameters.h"

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
    AlmostGL::WindingOrder windingOrder() { return GL.winding_order; }

signals:
    void windingOrderChanged();
    void FOVXchanged();
    void FOVYchanged();
 public slots:
    // Color Control
    void setAmbientR(float R) { model.material.ambient[0] = R; update_camera = true; }
    void setAmbientG(float G) { model.material.ambient[1] = G; update_camera = true; }
    void setAmbientB(float B) { model.material.ambient[2] = B; update_camera = true; }
    void setDiffuseR(float R) { model.material.diffuse[0] = R; update_camera = true; }
    void setDiffuseG(float G) { model.material.diffuse[1] = G; update_camera = true; }
    void setDiffuseB(float B) { model.material.diffuse[2] = B; update_camera = true; }
    void setSpecularR(float R) { model.material.specular[0] = R; update_camera = true; }
    void setSpecularG(float G) { model.material.specular[1] = G; update_camera = true; }
    void setSpecularB(float B) { model.material.specular[2] = B; update_camera = true; }
    void setShininess(float s) { model.material.shine = s; update_camera = true; }
    float getAmbientR() { return model.material.ambient[0]; }
    float getAmbientG() { return model.material.ambient[1]; }
    float getAmbientB() { return model.material.ambient[2]; }
    float getDiffuseR() { return model.material.diffuse[0]; }
    float getDiffuseG() { return model.material.diffuse[1]; }
    float getDiffuseB() { return model.material.diffuse[2]; }
    float getSpecularR() { return model.material.specular[0]; }
    float getSpecularG() { return model.material.specular[1]; }
    float getSpecularB() { return model.material.specular[2]; }
    float getShininess() { return model.material.shine; }
     
    void setAmbientLightR(float R) { GL.light.ambient_color[0] = R; update_camera = true; }
    void setAmbientLightG(float G) { GL.light.ambient_color[1] = G; update_camera = true; }
    void setAmbientLightB(float B) { GL.light.ambient_color[2] = B; update_camera = true; }
    void setSourceLightR(float R) { GL.light.sources[0].color[0] = R; update_camera = true; }
    void setSourceLightG(float G) { GL.light.sources[0].color[1] = G; update_camera = true; }
    void setSourceLightB(float B) { GL.light.sources[0].color[2] = B; update_camera = true; }
    float getAmbientLightR() { return GL.light.ambient_color[0]; }
    float getAmbientLightG() { return GL.light.ambient_color[1]; }
    float getAmbientLightB() { return GL.light.ambient_color[2]; }
    float getSourceLightR() { return GL.light.sources[0].color[0]; }
    float getSourceLightG() { return GL.light.sources[0].color[1]; }
    float getSourceLightB() { return GL.light.sources[0].color[2]; }

    // Lighting, vertex order and polygon mode(polygons,wireframe,etc)controls
    void setLightingMode(AlmostGL::LightingMode mode) { GL.light.mode = mode; update_camera = true; }
    void switchPolygonMode(AlmostGL::PolygonMode mode) { GL.polygon_mode = mode; }
    void setWindingOrder(AlmostGL::WindingOrder w) { GL.winding_order = w; update_camera = true; }

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
    AlmostGL::Pipeline GL;

    // Flags for keeping track of what needs updating
    bool fixed_center;
    bool update_camera;
    bool reset_camera;

    // Timer to refresh OpenGL window
    QTimer timer;
};

#endif // AlmostGLWindowWidget_h__