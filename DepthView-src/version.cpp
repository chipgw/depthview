#include "common.h"

#ifndef GIT_REV_MACRO
#define GIT_REV_MACRO "unknown"
#endif

namespace version{
    const int major = 1;
    const int minor = 02;
    const QString git_revision = GIT_REV_MACRO;

    QString getVersionString(){
        QString val = "%1.%2";
        return val.arg(major).arg(minor, 2, 10, QLatin1Char('0'));
    }
}
