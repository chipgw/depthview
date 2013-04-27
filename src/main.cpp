#include <QApplication>
#include <QDir>
#include "depthviewwindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    QDir::setCurrent(QDir::homePath());

    qDebug() << "DepthView version:" << qPrintable(version::getVersionString()) << "revision:" << qPrintable(version::git_revision) << "build type:" << version::build_type;

    app.setOrganizationName("DepthView");
    app.setApplicationName("DepthView");

    DepthViewWindow window;
    window.show();
    window.parseCommandLine(app.arguments());

    return app.exec();
}
