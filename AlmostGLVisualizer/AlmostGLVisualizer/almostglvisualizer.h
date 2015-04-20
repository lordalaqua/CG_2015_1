#ifndef ALMOSTGLVISUALIZER_H
#define ALMOSTGLVISUALIZER_H

#include <QtWidgets/QMainWindow>
#include "ui_almostglvisualizer.h"

class AlmostGLVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    AlmostGLVisualizer(QWidget *parent = 0);
    ~AlmostGLVisualizer() {};

private:
    void setupOpenGLWidgetConnections();
    void setupAlmostGLWidgetConnections();
    Ui::AlmostGLVisualizerClass ui;
};

#endif // ALMOSTGLVISUALIZER_H
