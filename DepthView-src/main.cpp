#include <QtGui/QApplication>
#include "depthviewwindow.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    qDebug() << "DepthView version:" << qPrintable(version::getVersionString()) << "revision:" << qPrintable(version::svn_revision);

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
