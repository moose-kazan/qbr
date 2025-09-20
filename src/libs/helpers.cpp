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

  const qreal desktopDpi = desktop->physicalDotsPerInch();
  return desktopDpi > 72 ? desktopDpi / 72 : 1;
}

int Helpers::getDesktopScaledInt(const int n) {
  const int rv = round(getDesktopScale() * n);
  return rv;
}

QString Helpers::getDesktopScaledIntAsQString(const int n) {
  QString rv;
  rv.setNum(getDesktopScaledInt(n));
  return rv;
}
