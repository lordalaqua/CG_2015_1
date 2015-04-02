#ifndef OPENGLVISUALIZER_H
#define OPENGLVISUALIZER_H

#include <QtWidgets/QMainWindow>
#include "ui_openglvisualizer.h"

class OpenGLVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    OpenGLVisualizer(QWidget *parent = 0);
    ~OpenGLVisualizer();

private:
    void setupConnections();
    Ui::OpenGLVisualizerClass ui;
};

#endif // OPENGLVISUALIZER_H
