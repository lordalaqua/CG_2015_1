#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RayTracer.h"
#include <string>

class RayTracerApp : public QMainWindow
{
    Q_OBJECT

public:
    RayTracerApp(std::string input, QWidget *parent = 0);
    ~RayTracerApp();

private:
    Ui::RayTracerClass ui;
};
