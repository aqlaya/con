#include <QApplication>
#include <QMainWindow>
#include <QQmlApplicationEngine>
#include <QPushButton>
#include <qapplication.h>
#include <qqmlapplicationengine.h>

int main(int argc, char *argv[])
{
    // Initialize the application
    QApplication app(argc, argv);
    QQmlApplicationEngine appEngine;
    appEngine.load("qrc:/UI/MainWindow.qml");

    // Run the application
    return app.exec();
}