#ifndef ALMOSTGLVISUALIZER_H
#define ALMOSTGLVISUALIZER_H

#include <QtWidgets/QMainWindow>
#include "ui_almostglvisualizer.h"

/*
    Main Application window class

    Holds the user interface designed in Qt Designer and inherits from
    Qt modules for generating the main window of the GUI.

    Setups connections between interface elements and functions in OpenGL
    and AlmostGL widgets.
*/

class AlmostGLVisualizer : public QMainWindow
{
    Q_OBJECT

public:
    AlmostGLVisualizer(QWidget *parent = 0);
    ~AlmostGLVisualizer() {};

private:
    void setupWidgetConnections();
    Ui::AlmostGLVisualizerClass ui;
};

#endif // ALMOSTGLVISUALIZER_H
