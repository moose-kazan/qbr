#include "helpers.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>
#include <QString>
#include <cmath>

Helpers::Helpers() = default;

qreal Helpers::getDesktopScale() {
  // Fix for HiDpi screens
  // By default X.Org have 72 dpi
  const QScreen *desktop = QApplication::screens().at(0);

  qreal desktopDpi = desktop->physicalDotsPerInch();
  return desktopDpi > 72 ? desktopDpi / 72 : 1;
}

int Helpers::getDesktopScaledInt(int n) {
  int rv = round(getDesktopScale() * n);
  return rv;
}

QString Helpers::getDesktopScaledIntAsQString(int n) {
  QString rv;
  rv.setNum(getDesktopScaledInt(n));
  return rv;
}
