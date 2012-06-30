#include "common.h"

#ifndef SVN_REV_MACRO
#define SVN_REV_MACRO "unknown"
#endif

namespace version{
    const int major = 1;
    const int minor = 00;
    const QString svn_revision = SVN_REV_MACRO;

    QString getVersionString(){
        QString val = "%1.%2";
        return val.arg(major).arg(minor, 2, 10, QLatin1Char('0'));
    }
}
