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
    //================ Color input sliders =====================================
    connect(ui.sliderRed, &QSlider::valueChanged, [&]
    { 
        ui.almostGLWidget->setColorR(ui.sliderRed->value() / 255.0); 
        ui.openGLWidget->setColorR(ui.sliderRed->value() / 255.0);
    });
    connect(ui.sliderGreen, &QSlider::valueChanged, [&]
    { 
        ui.almostGLWidget->setColorG(ui.sliderGreen->value() / 255.0);
        ui.openGLWidget->setColorG(ui.sliderGreen->value() / 255.0);
    });
    connect(ui.sliderBlue, &QSlider::valueChanged, [&]
    { 
        ui.almostGLWidget->setColorB(ui.sliderBlue->value() / 255.0);
        ui.openGLWidget->setColorB(ui.sliderBlue->value() / 255.0);
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
        ui.openGLWidget->windingOrder() == GL_CW ? 
            ui.radioButtonCW->setChecked(true) : 
            ui.radioButtonCCW->setChecked(true); 
    });

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
    connect(ui.lineEditNear, &QLineEdit::textChanged, [&]
    {
        ui.almostGLWidget->setZNear(ui.lineEditNear->text().toFloat());
        ui.openGLWidget->setZNear(ui.lineEditNear->text().toFloat());
    });
    connect(ui.lineEditFar, &QLineEdit::textChanged, [&]
    {
        ui.almostGLWidget->setZFar(ui.lineEditFar->text().toFloat());
        ui.openGLWidget->setZFar(ui.lineEditFar->text().toFloat());
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
