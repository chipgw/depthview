#include "depthviewwindow.h"
#include "version.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    app.setOrganizationName("chipgw");
    app.setApplicationName("DepthView");
    app.setApplicationVersion(version::getVersionString());

    if(app.arguments().contains("--register")){
        DepthViewWindow::registerFileTypes();
        return 0;
    }

    if(QDir::currentPath() == app.applicationDirPath()){
        QDir::setCurrent(QDir::homePath());
    }

    DepthViewWindow window;
    window.show();
    window.parseCommandLine(app.arguments());

    return app.exec();
}
