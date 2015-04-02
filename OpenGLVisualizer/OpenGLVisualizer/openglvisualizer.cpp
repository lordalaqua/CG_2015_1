#include "openglvisualizer.h"
#include <QFileDialog>

OpenGLVisualizer::OpenGLVisualizer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setupConnections();
}

OpenGLVisualizer::~OpenGLVisualizer()
{

}

void OpenGLVisualizer::setupConnections()
{
    connect(ui.radioButtonFill, &QRadioButton::toggled, [&]{ui.openGLWidget->switchRenderMode(GL_FILL); });
    connect(ui.radioButtonLine, &QRadioButton::toggled, [&]{ui.openGLWidget->switchRenderMode(GL_LINE); });
    connect(ui.radioButtonPoint, &QRadioButton::toggled, [&]{ui.openGLWidget->switchRenderMode(GL_POINT); });
    connect(ui.radioButtonCW, &QRadioButton::toggled, [&]{if (ui.radioButtonCW->isChecked()) ui.openGLWidget->setWindingOrder(GL_CW); });
    connect(ui.radioButtonCCW, &QRadioButton::toggled, [&]{if(ui.radioButtonCCW->isChecked()) ui.openGLWidget->setWindingOrder(GL_CCW); });
    connect(ui.sliderRed, &QSlider::valueChanged, [&]{ ui.openGLWidget->setColorR(ui.sliderRed->value() / 255.0); });
    connect(ui.sliderGreen, &QSlider::valueChanged, [&]{ ui.openGLWidget->setColorG(ui.sliderGreen->value() / 255.0); });
    connect(ui.sliderBlue, &QSlider::valueChanged, [&]{ ui.openGLWidget->setColorB(ui.sliderBlue->value() / 255.0); });
    connect(ui.buttonLoadFile, &QPushButton::clicked, [&]{ 
        QString filename = QFileDialog::getOpenFileName(this, tr("Open Model File..."), QString());
        ui.openGLWidget->loadModel(filename.toStdString());
        update();
    });
    connect(ui.checkBoxLighting, &QCheckBox::stateChanged, [&]{ui.openGLWidget->toggleLighting(ui.checkBoxLighting->isChecked());});
    connect(ui.lineEditNear, &QLineEdit::textChanged, [&]{ui.openGLWidget->setZNear(ui.lineEditNear->text().toFloat()); });
    connect(ui.lineEditFar, &QLineEdit::textChanged, [&]{ui.openGLWidget->setZFar(ui.lineEditFar->text().toFloat()); });
    connect(ui.sliderFOVX, &QSlider::valueChanged, [&]{ ui.openGLWidget->setFOVX(ui.sliderFOVX->value()); });
    connect(ui.sliderFOVY, &QSlider::valueChanged, [&]{ ui.openGLWidget->setFOVY(ui.sliderFOVY->value()); });
    connect(ui.sliderTransX, &QSlider::valueChanged, [&] { ui.openGLWidget->translateCameraX(ui.sliderTransX->value()); });
    connect(ui.sliderTransY, &QSlider::valueChanged, [&] { ui.openGLWidget->translateCameraY(ui.sliderTransY->value()); });
    connect(ui.sliderTransZ, &QSlider::valueChanged, [&] { ui.openGLWidget->translateCameraZ(ui.sliderTransZ->value()); });
    connect(ui.sliderRotX, &QSlider::valueChanged, [&] { ui.openGLWidget->rotateCameraX(ui.sliderRotX->value()); });
    connect(ui.sliderRotY, &QSlider::valueChanged, [&] { ui.openGLWidget->rotateCameraY(ui.sliderRotY->value()); });
    connect(ui.sliderRotZ, &QSlider::valueChanged, [&] { ui.openGLWidget->rotateCameraZ(ui.sliderRotZ->value()); });
    connect(ui.buttonResetCamera, &QPushButton::clicked, [&]{ui.openGLWidget->setCameraReset(); });
    connect(ui.checkBoxFixedCenter, &QCheckBox::stateChanged, [&]{ui.openGLWidget->setFixedCenter(ui.checkBoxFixedCenter->isChecked()); });
}
