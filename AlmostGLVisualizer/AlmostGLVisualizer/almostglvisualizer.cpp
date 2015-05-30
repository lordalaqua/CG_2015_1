#include "almostglvisualizer.h"
#include <QFileDialog>

AlmostGLVisualizer::AlmostGLVisualizer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setupWidgetConnections();
}

void AlmostGLVisualizer::setupWidgetConnections()
{
    //================ Material input sliders ==================================
    connect(ui.sliderRed, &QSlider::valueChanged, [&]
    {
        if (ui.ambientButton->isChecked())
        {
            ui.almostGLWidget->setAmbientR(ui.sliderRed->value() / 255.0);
            ui.openGLWidget->setAmbientR(ui.sliderRed->value() / 255.0);
        }
        else if (ui.diffuseButton->isChecked())
        {
            ui.almostGLWidget->setDiffuseR(ui.sliderRed->value() / 255.0);
            ui.openGLWidget->setDiffuseR(ui.sliderRed->value() / 255.0);
        }
        else if (ui.specularButton->isChecked())
        {
            ui.almostGLWidget->setSpecularR(ui.sliderRed->value() / 255.0);
            ui.openGLWidget->setSpecularR(ui.sliderRed->value() / 255.0);
        }
    });
    connect(ui.sliderGreen, &QSlider::valueChanged, [&]
    { 
        if (ui.ambientButton->isChecked())
        {
            ui.almostGLWidget->setAmbientG(ui.sliderGreen->value() / 255.0);
            ui.openGLWidget->setAmbientG(ui.sliderGreen->value() / 255.0);
        }
        else if (ui.diffuseButton->isChecked())
        {
            ui.almostGLWidget->setDiffuseG(ui.sliderGreen->value() / 255.0);
            ui.openGLWidget->setDiffuseG(ui.sliderGreen->value() / 255.0);
        }
        else if (ui.specularButton->isChecked())
        {
            ui.almostGLWidget->setSpecularG(ui.sliderGreen->value() / 255.0);
            ui.openGLWidget->setSpecularG(ui.sliderGreen->value() / 255.0);
        }
    });
    connect(ui.sliderBlue, &QSlider::valueChanged, [&]
    { 
        if (ui.ambientButton->isChecked())
        {
            ui.almostGLWidget->setAmbientB(ui.sliderBlue->value() / 255.0);
            ui.openGLWidget->setAmbientB(ui.sliderBlue->value() / 255.0);
        }
        else if (ui.diffuseButton->isChecked())
        {
            ui.almostGLWidget->setDiffuseB(ui.sliderBlue->value() / 255.0);
            ui.openGLWidget->setDiffuseB(ui.sliderBlue->value() / 255.0);
        }
        else if (ui.specularButton->isChecked())
        {
            ui.almostGLWidget->setSpecularB(ui.sliderBlue->value() / 255.0);
            ui.openGLWidget->setSpecularB(ui.sliderBlue->value() / 255.0);
        }
    });
    connect(ui.shininessSlider, &QSlider::valueChanged, [&]
    {
        ui.almostGLWidget->setShininess(ui.shininessSlider->value());
        ui.openGLWidget->setShininess(ui.shininessSlider->value());
    });

    connect(ui.ambientButton, &QRadioButton::toggled, [&] 
    {
        if (ui.ambientButton->isChecked())
        {
            ui.sliderRed->setValue(ui.almostGLWidget->getAmbientR()*255.0);
            ui.sliderGreen->setValue(ui.almostGLWidget->getAmbientG()*255.0);
            ui.sliderBlue->setValue(ui.almostGLWidget->getAmbientB()*255.0);
        }
    });

    connect(ui.diffuseButton, &QRadioButton::toggled, [&]
    {
        if (ui.diffuseButton->isChecked())
        {
            ui.sliderRed->setValue(ui.almostGLWidget->getDiffuseR()*255.0);
            ui.sliderGreen->setValue(ui.almostGLWidget->getDiffuseG()*255.0);
            ui.sliderBlue->setValue(ui.almostGLWidget->getDiffuseB()*255.0);
        }
    });

    connect(ui.specularButton, &QRadioButton::toggled, [&]
    {
        if (ui.specularButton->isChecked())
        {
            ui.sliderRed->setValue(ui.almostGLWidget->getSpecularR()*255.0);
            ui.sliderGreen->setValue(ui.almostGLWidget->getSpecularG()*255.0);
            ui.sliderBlue->setValue(ui.almostGLWidget->getSpecularB()*255.0);
        }
    });

    //==================== Enable Lighting Checkbox ============================
    connect(ui.checkBoxLighting, &QCheckBox::stateChanged, [&]
    {
        ui.openGLWidget->toggleLighting(ui.checkBoxLighting->isChecked());
        if (!ui.checkBoxLighting->isChecked())
        {
            ui.almostGLWidget->setLightingMode(AlmostGL::NONE);
        }
        else
        {
            if (ui.radioButtonFlat->isChecked())
                ui.almostGLWidget->setLightingMode(AlmostGL::FLAT);
            else
                ui.almostGLWidget->setLightingMode(AlmostGL::SMOOTH);
        }
    });

    //================== Lighting mode radio buttons ===========================
    connect(ui.radioButtonFlat, &QRadioButton::toggled, [&]
    {
        if (ui.radioButtonFlat->isChecked())
        {
            ui.openGLWidget->setLightingMode(GL_FLAT);
            if (ui.checkBoxLighting->isChecked())
                ui.almostGLWidget->setLightingMode(AlmostGL::FLAT);
        }
    });
    connect(ui.radioButtonSmooth, &QRadioButton::toggled, [&]
    {
        if (ui.radioButtonSmooth->isChecked())
        {
            ui.openGLWidget->setLightingMode(GL_SMOOTH);
            if (ui.checkBoxLighting->isChecked())
                ui.almostGLWidget->setLightingMode(AlmostGL::SMOOTH);
        }
    });

    //==================== Light Color Controls ================================

    connect(ui.redLightSlider, &QSlider::valueChanged, [&]
    {
        if (ui.ambientLightButton->isChecked())
        {
            ui.almostGLWidget->setAmbientLightR(ui.redLightSlider->value() / 255.0);
            ui.openGLWidget->setAmbientLightR(ui.redLightSlider->value() / 255.0);
        }
        else if (ui.lightSourceButton->isChecked())
        {
            ui.almostGLWidget->setSourceLightR(ui.redLightSlider->value() / 255.0);
            ui.openGLWidget->setSourceLightR(ui.redLightSlider->value() / 255.0);
        }
    });

    connect(ui.greenLightSlider, &QSlider::valueChanged, [&]
    {
        if (ui.ambientLightButton->isChecked())
        {
            ui.almostGLWidget->setAmbientLightG(ui.greenLightSlider->value() / 255.0);
            ui.openGLWidget->setAmbientLightG(ui.greenLightSlider->value() / 255.0);
        }
        else if (ui.lightSourceButton->isChecked())
        {
            ui.almostGLWidget->setSourceLightG(ui.greenLightSlider->value() / 255.0);
            ui.openGLWidget->setSourceLightG(ui.greenLightSlider->value() / 255.0);
        }
    });

    connect(ui.blueLightSlider, &QSlider::valueChanged, [&]
    {
        if (ui.ambientLightButton->isChecked())
        {
            ui.almostGLWidget->setAmbientLightB(ui.blueLightSlider->value() / 255.0);
            ui.openGLWidget->setAmbientLightB(ui.blueLightSlider->value() / 255.0);
        }
        else if (ui.lightSourceButton->isChecked())
        {
            ui.almostGLWidget->setSourceLightB(ui.blueLightSlider->value() / 255.0);
            ui.openGLWidget->setSourceLightB(ui.blueLightSlider->value() / 255.0);
        }
    });

    connect(ui.ambientLightButton, &QRadioButton::toggled, [&]
    {
        if (ui.ambientLightButton->isChecked())
        {
            ui.redLightSlider->setValue(ui.almostGLWidget->getAmbientLightR()*255.0);
            ui.greenLightSlider->setValue(ui.almostGLWidget->getAmbientLightG()*255.0);
            ui.blueLightSlider->setValue(ui.almostGLWidget->getAmbientLightB()*255.0);
        }
    });

    connect(ui.lightSourceButton, &QRadioButton::toggled, [&]
    {
        if (ui.lightSourceButton->isChecked())
        {
            ui.redLightSlider->setValue(ui.almostGLWidget->getSourceLightR()*255.0);
            ui.greenLightSlider->setValue(ui.almostGLWidget->getSourceLightG()*255.0);
            ui.blueLightSlider->setValue(ui.almostGLWidget->getSourceLightB()*255.0);
        }
    });


    //==================== Vertex Order Radio buttons ==========================
    connect(ui.radioButtonCW, &QRadioButton::toggled, [&]
    {
        if (ui.radioButtonCW->isChecked())
        {
            ui.almostGLWidget->setWindingOrder(AlmostGL::CW);
            ui.openGLWidget->setWindingOrder(GL_CW);
        }
    });
    connect(ui.radioButtonCCW, &QRadioButton::toggled, [&]
    {
        if (ui.radioButtonCCW->isChecked()) 
        {
            ui.almostGLWidget->setWindingOrder(AlmostGL::CCW);
            ui.openGLWidget->setWindingOrder(GL_CCW);
        }
    });
    connect(ui.almostGLWidget, &AlmostGLWindowWidget::windingOrderChanged, [&]
    {   
        ui.almostGLWidget->windingOrder() == AlmostGL::CW ? 
            ui.radioButtonCW->setChecked(true) : 
            ui.radioButtonCCW->setChecked(true);
    });
//     connect(ui.openGLWidget, &OpenGLWindowWidget::windingOrderChanged, [&]
//     {
//         ui.openGLWidget->windingOrder() == GL_CW ?
//             ui.radioButtonCW->setChecked(true) :
//             ui.radioButtonCCW->setChecked(true);
//     });

    //=================== Polygon mode radio buttons ===========================
    connect(ui.radioButtonFill, &QRadioButton::toggled, [&]
    {
        ui.almostGLWidget->switchPolygonMode(AlmostGL::FILL);
        ui.openGLWidget->switchPolygonMode(GL_FILL); 
    });
    connect(ui.radioButtonLine, &QRadioButton::toggled, [&]
    {
        ui.almostGLWidget->switchPolygonMode(AlmostGL::WIREFRAME);
        ui.openGLWidget->switchPolygonMode(GL_LINE); 
    });
    connect(ui.radioButtonPoint, &QRadioButton::toggled, [&]
    {
        ui.almostGLWidget->switchPolygonMode(AlmostGL::POINTS);
        ui.openGLWidget->switchPolygonMode(GL_POINT); 
    });

    //================== Load model file dialog button =========================
    connect(ui.buttonLoadFile, &QPushButton::clicked, [&]{
        QString filename = QFileDialog::getOpenFileName(this, 
            tr("Open Model File..."), QString());
        ui.almostGLWidget->loadModel(filename.toStdString());
        ui.openGLWidget->loadModel(filename.toStdString());
    });

    //======================= Znear and Zfar plane controls ====================
    connect(ui.nearSlider, &QSlider::valueChanged, [&]
    {
        ui.almostGLWidget->setZNear(ui.nearSlider->value());
        ui.openGLWidget->setZNear(ui.nearSlider->value());
    });
    connect(ui.farSlider, &QSlider::valueChanged, [&]
    {
        ui.almostGLWidget->setZFar(ui.farSlider->value());
        ui.openGLWidget->setZFar(ui.farSlider->value());
    });

    //========================= Field of view controls =========================
    connect(ui.sliderFOVX, &QSlider::valueChanged, [&]
    { 
        ui.almostGLWidget->setFOVX(ui.sliderFOVX->value()); 
        ui.openGLWidget->setFOVX(ui.sliderFOVX->value());
    });
    connect(ui.sliderFOVY, &QSlider::valueChanged, [&]
    { 
        ui.almostGLWidget->setFOVY(ui.sliderFOVY->value());
        ui.openGLWidget->setFOVY(ui.sliderFOVY->value());
    });
    connect(ui.openGLWidget, &OpenGLWindowWidget::FOVXchanged, [&]
    {
        ui.sliderFOVX->setValue(ui.almostGLWidget->fieldOfViewX());
        //ui.sliderFOVX->setValue(ui.openGLWidget->fieldOfViewX());
    });
    connect(ui.openGLWidget, &OpenGLWindowWidget::FOVYchanged, [&]
    {
        ui.sliderFOVY->setValue(ui.almostGLWidget->fieldOfViewY());
        //ui.sliderFOVY->setValue(ui.openGLWidget->fieldOfViewY());
    });

    //========================= Reset Camera button ============================
    connect(ui.buttonResetCamera, &QPushButton::clicked, [&]
    {
        ui.almostGLWidget->setCameraReset();
        ui.openGLWidget->setCameraReset(); 
    });
 
    //===================== Fixed center translation checkbox ==================
    connect(ui.checkBoxFixedCenter, &QCheckBox::stateChanged, [&]
    {
        ui.almostGLWidget->setFixedCenter(ui.checkBoxFixedCenter->isChecked());
        ui.openGLWidget->setFixedCenter(ui.checkBoxFixedCenter->isChecked());
    });
 
    //========================== Translation sliders ===========================
    connect(ui.sliderTransX, &QSlider::valueChanged, [&] 
    { 
        ui.almostGLWidget->translateCameraX(ui.sliderTransX->value());
        ui.openGLWidget->translateCameraX(ui.sliderTransX->value());
    });
    connect(ui.sliderTransX, &QSlider::sliderReleased, [&] 
    { 
        ui.sliderTransX->setValue(0); 
    });
    connect(ui.sliderTransY, &QSlider::valueChanged, [&] 
    { 
        ui.almostGLWidget->translateCameraY(ui.sliderTransY->value());
        ui.openGLWidget->translateCameraY(ui.sliderTransY->value());
    });
    connect(ui.sliderTransY, &QSlider::sliderReleased, [&] 
    { 
        ui.sliderTransY->setValue(0); 
    });
    connect(ui.sliderTransZ, &QSlider::valueChanged, [&] 
    { 
        ui.almostGLWidget->translateCameraZ(ui.sliderTransZ->value());
        ui.openGLWidget->translateCameraZ(ui.sliderTransZ->value());
    });
    connect(ui.sliderTransZ, &QSlider::sliderReleased, [&] 
    { 
        ui.sliderTransZ->setValue(0); 
    });
 
    //========================= Rotation sliders ===============================
    connect(ui.sliderRotX, &QSlider::valueChanged, [&] 
    { 
        ui.almostGLWidget->rotateCameraX(ui.sliderRotX->value());
        ui.openGLWidget->rotateCameraX(ui.sliderRotX->value());
    });
    connect(ui.sliderRotX, &QSlider::sliderReleased, [&] 
    { 
        ui.sliderRotX->setValue(0);
    });
    connect(ui.sliderRotY, &QSlider::valueChanged, [&] 
    { 
        ui.almostGLWidget->rotateCameraY(ui.sliderRotY->value());
        ui.openGLWidget->rotateCameraY(ui.sliderRotY->value());
    });
    connect(ui.sliderRotY, &QSlider::sliderReleased, [&] 
    { 
        ui.sliderRotY->setValue(0);
    });
    connect(ui.sliderRotZ, &QSlider::valueChanged, [&] 
    { 
        ui.almostGLWidget->rotateCameraZ(ui.sliderRotZ->value());
        ui.openGLWidget->rotateCameraZ(ui.sliderRotZ->value());
    });
    connect(ui.sliderRotZ, &QSlider::sliderReleased, [&] 
    { 
        ui.sliderRotZ->setValue(0);
    });
}
