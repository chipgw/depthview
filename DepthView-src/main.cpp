#include <QtGui/QApplication>
#include "depthviewwindow.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

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
