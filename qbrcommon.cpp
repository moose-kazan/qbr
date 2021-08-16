#include "qbrcommon.h"

#include <math.h>
#include <QDesktopWidget>
#include <QString>
#include <QDebug>

qbrcommon::qbrcommon()
{

}

float qbrcommon::getDesktopScale()
{
    // Fix for HiDpi screens
    // By default X.Org have 72 dpi
    QDesktopWidget desktop;

    int desktopDpi = desktop.logicalDpiY();
    return desktopDpi > 72 ? desktopDpi/72 : 1;
}

int qbrcommon::getDesktopScaledInt(int n)
{
    int rv = round(getDesktopScale() * n);
    return rv;
}

QString qbrcommon::getDesktopScaledIntAsQString(int n)
{
    QString rv;
    rv.setNum(getDesktopScaledInt(n));
    return rv;
}
