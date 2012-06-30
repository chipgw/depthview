#ifndef COMMON_H
#define COMMON_H

#include <QImage>
#include <QDebug>
#include <QWidget>
#include <QDir>

namespace version{
    extern const int major;
    extern const int minor;
    extern const QString svn_revision; // not an integer because of possible "#M" for modified or "#:#" for multiple versions.
    QString getVersionString();
}

#endif // COMMON_H
