#ifndef OpenGLWindowWidget_h__
#define OpenGLWindowWidget_h__

#include "QTWidgets/QOpenGLWidget"
#include "QOpenGLFunctions"
#include "QOpenGLTexture"
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
    void setAmbientR(float R) { model.material.ambient[0] = R;}
    void setAmbientG(float G) { model.material.ambient[1] = G;}
    void setAmbientB(float B) { model.material.ambient[2] = B;}
    void setDiffuseR(float R) { model.material.diffuse[0] = R;}
    void setDiffuseG(float G) { model.material.diffuse[1] = G;}
    void setDiffuseB(float B) { model.material.diffuse[2] = B;}
    void setSpecularR(float R) { model.material.specular[0] = R; }
    void setSpecularG(float G) { model.material.specular[1] = G; }
    void setSpecularB(float B) { model.material.specular[2] = B; }
    void setShininess(float s) { model.material.shine = s;  }
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

    void setAmbientLightR(float R) { ambient_light[0] = R; update_lighting = true;}
    void setAmbientLightG(float G) { ambient_light[1] = G; update_lighting = true;}
    void setAmbientLightB(float B) { ambient_light[2] = B; update_lighting = true;}
    void setSourceLightR(float R) { light_color[0] = R; update_lighting = true; }
    void setSourceLightG(float G) { light_color[1] = G; update_lighting = true; }
    void setSourceLightB(float B) { light_color[2] = B; update_lighting = true; }
    float getAmbientLightR() { return ambient_light[0];}
    float getAmbientLightG() { return ambient_light[1];}
    float getAmbientLightB() { return ambient_light[2];}
    float getSourceLightR() { return light_color[0]; }
    float getSourceLightG() { return light_color[1]; }
    float getSourceLightB() { return light_color[2]; }

    void toggleTextureMapping(bool v) { texture_on = v; }
    void setTextureMapMode(GLenum m) { tex_mode = m; }
    void setTextureMapMinFilter(QOpenGLTexture::Filter f) { min_filter = f; }
    void setTextureMapMagFilter(QOpenGLTexture::Filter f) { mag_filter = f; }

    // Lighting, vertex order and polygon mode(polygons,wireframe,etc)controls
    void toggleLighting(bool is_on);
    void setLightingMode(GLenum mode);
    void setWindingOrder(GLenum w);
    void switchPolygonMode(GLenum mode);

    // Model load interface
    void loadModel(std::string filename);

    // Texture load
    void loadTexture(std::string filename);
    
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
    GLfloat light_position[4];

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

    // Texture
    QOpenGLTexture texture;
    bool texture_loaded;
    bool texture_on;
    bool update_texture;
    QOpenGLTexture::Filter min_filter;
    QOpenGLTexture::Filter mag_filter;
    GLenum tex_mode;

    // Flags for keeping track of what needs updating
    bool update_camera;
    bool update_order;
    bool reset_camera;
    bool update_lighting;

    // Timer to refresh OpenGL window
    QTimer timer;
};

#endif // OpenGLWindowWidget_h__