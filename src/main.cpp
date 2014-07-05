#include "depthviewwindow.h"
#include "version.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    if(QDir::currentPath() == QApplication::applicationDirPath()){
        QDir::setCurrent(QDir::homePath());
    }

    app.setOrganizationName("chipgw");
    app.setApplicationName("DepthView");
    app.setApplicationVersion(version::getVersionString());

    if(app.arguments().contains("--register")){
        DepthViewWindow::registerFileTypes();
        return 0;
    }
    DepthViewWindow window;
    window.show();
    window.parseCommandLine(app.arguments());

    return app.exec();
}
