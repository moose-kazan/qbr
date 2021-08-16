#ifndef QBRCOMMON_H
#define QBRCOMMON_H

#include <QString>


class qbrcommon
{
public:
    qbrcommon();
    static float getDesktopScale();
    static int getDesktopScaledInt(int n);
    static QString getDesktopScaledIntAsQString(int n);
};

#endif // QBRCOMMON_H
