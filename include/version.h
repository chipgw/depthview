#pragma once

#include <QString>

namespace version{
    extern const int major;
    extern const int minor;
    extern const QString git_revision;
    QString getVersionString();
    extern const QString build_type;
}
