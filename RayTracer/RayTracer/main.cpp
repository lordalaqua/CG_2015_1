#include "RayTracerApp.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <iostream>
#include <windows.h>

void createConsoleLog(LPCWSTR title) {

    AllocConsole();
    SetConsoleTitle(title);

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

void myMessageHandler(QtMsgType type, const QMessageLogContext &context,
    const QString &msg) {
    std::cout << msg.toStdString() << std::endl;
}

int main(int argc, char *argv[])
{
#ifdef _DEBUG
    qInstallMessageHandler(&myMessageHandler);
    createConsoleLog(L"RayTracer");
#endif
//     std::cout << "Computer Graphics - Raytracer" << std::endl << std::endl;
//     if (argc < 2 || argc > 3) {
//         std::cerr << "Usage: " << argv[0] << " input-file [output-file.png]" << std::endl;
//         return 1;
//     }
    std::string filename = "./Resources/_Planets.yaml";// argv[1];
    QApplication a(argc, argv);
    RayTracerApp w(filename);
    w.show();
    return a.exec();
}
