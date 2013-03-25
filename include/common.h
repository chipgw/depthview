#ifndef COMMON_H
#define COMMON_H

#include <QImage>
#include <QDebug>
#include <QWidget>
#include <QDir>

namespace version{
    extern const int major;
    extern const int minor;
    extern const QString git_revision;
    QString getVersionString();
    extern const QString build_type;
}

#endif // COMMON_H
