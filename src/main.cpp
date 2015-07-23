#include "depthviewwindow.h"
#include "version.h"
#include <QApplication>
#include <QDir>
#include <QCommandLineParser>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    app.setOrganizationName("chipgw");
    app.setApplicationName("DepthView");
    app.setApplicationVersion(version::getVersionString());

    QCommandLineParser cmdline;

    cmdline.addOptions({
            {"register",            QCoreApplication::translate("main", "")},
            {{"f", "fullscreen"},   QCoreApplication::translate("main", "")},
            {{"d", "startdir"},
                QCoreApplication::translate("main", ""),
                QCoreApplication::translate("main", "directory")},
            {{"r", "renderer"},
                QCoreApplication::translate("main", ""),
                QCoreApplication::translate("main", "renderer")},
        });

    cmdline.parse(app.arguments());

    if(cmdline.isSet("register")){
        DepthViewWindow::registerFileTypes();
        return 0;
    }

    if(QDir::currentPath() == app.applicationDirPath())
        QDir::setCurrent(QDir::homePath());

    DepthViewWindow window;
    window.show();
    window.parseCommandLine(cmdline);

    return app.exec();
}
