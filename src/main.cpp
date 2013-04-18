#include <QApplication>
#include "depthviewwindow.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QDir::setCurrent(QDir::homePath());

    qDebug() << "DepthView version:" << qPrintable(version::getVersionString()) << "revision:" << qPrintable(version::git_revision) << "build type:" << version::build_type;

    a.setOrganizationName("DepthView");
    a.setApplicationName("DepthView");

    DepthViewWindow w;
    w.show();

    if (argc > 1){
        if(!w.loadImage(argv[1])){
            w.showLoadImageDialog();
        }
    }
    else{
        w.showLoadImageDialog();
    }

    return a.exec();
}