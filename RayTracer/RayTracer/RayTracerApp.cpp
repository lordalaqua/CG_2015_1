#include "RayTracerApp.h"
#include "Scene.h"
#include <QImage>


RayTracerApp::RayTracerApp(std::string input, QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);   
    
    // Parse Input File
    Scene scene(input);

    // Trace Rays and generate image
     QImage image;
     scene.render(image);
 
     // Save and Display Image
     image.save("test.png");
}

RayTracerApp::~RayTracerApp()
{
}
