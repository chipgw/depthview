#include "depthviewwindow.h"
#include "version.h"
#include <QApplication>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    if(QDir::currentPath() == QApplication::applicationDirPath()){
        QDir::setCurrent(QDir::homePath());
    }

    qDebug() << "DepthView version:" << version::getVersionString()
             << "revision:" << version::git_revision
             << "build type:" << version::build_type;

    app.setOrganizationName("chipgw");
    app.setApplicationName("DepthView");
    app.setApplicationVersion(version::getVersionString());

    DepthViewWindow window;
    window.show();
    window.parseCommandLine(app.arguments());

    return app.exec();
}
