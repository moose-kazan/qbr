#include "qbrcommon.h"

#include <QDebug>
#include <QScreen>
#include <QString>
#include <QApplication>
#include <math.h>

qbrcommon::qbrcommon() {}

float qbrcommon::getDesktopScale() {
  // Fix for HiDpi screens
  // By default X.Org have 72 dpi
  QScreen *desktop = QApplication::screens().at(0);

  int desktopDpi = desktop->physicalDotsPerInch();
  return desktopDpi > 72 ? desktopDpi / 72 : 1;
}

int qbrcommon::getDesktopScaledInt(int n) {
  int rv = round(getDesktopScale() * n);
  return rv;
}

QString qbrcommon::getDesktopScaledIntAsQString(int n) {
  QString rv;
  rv.setNum(getDesktopScaledInt(n));
  return rv;
}
