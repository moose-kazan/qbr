#include "settings.h"
#include <QSettings>

Settings::Settings() {}

QSettings *Settings::cfg = nullptr;

QSettings *Settings::getInstance() {
  if (cfg == nullptr) {
    cfg = new QSettings("YLSoftware", "QBR");
  }
  return cfg;
}

bool Settings::getStatusBarEnabled() {
  return getInstance()->value("statusBarEnabled", true).toBool();
}
void Settings::setStatusBarEnabled(const bool enabled) {
  getInstance()->setValue("statusBarEnabled", enabled);
}

int Settings::getUiVariant() {
  return getInstance()->value("uiVariant", 0).toInt();
}

void Settings::setUiVariant(int uiVaraint) {
  getInstance()->setValue("uiVariant", uiVaraint);
}

void Settings::setFilePosition(const QString& fileName, const QPointF pos) {
  QString cfgkey = "filePos_" + fileName;
  getInstance()->setValue(cfgkey, pos);
}

QPointF Settings::getFilePosition(const QString& fileName) {
  QString cfgkey = "filePos_" + fileName;
  if (const QSettings *cfg = getInstance(); cfg->contains(cfgkey)) {
    return getInstance()->value(cfgkey).toPointF();
  }
  return QPointF(0, 0);
}

QByteArray Settings::getMainWindowState(QByteArray state) {
  if (const QSettings *cfg = getInstance(); cfg->contains("mainWindowState")) {
    return cfg->value("mainWindowState").toByteArray();
  }
  return state;
}

void Settings::setMainWindowState(const QByteArray& state) {
  getInstance()->setValue("mainWindowState", state);
}

QByteArray Settings::getMainWindowGeometry(QByteArray geometry) {
  if (const QSettings *cfg = getInstance(); cfg->contains("mainWindowGeometry")) {
    return cfg->value("mainWindowGeometry").toByteArray();
  }
  return geometry;
}

void Settings::setMainWindowGeometry(const QByteArray& geometry) {
  getInstance()->setValue("mainWindowGeometry", geometry);
}

QString Settings::getLastOpenedFile() {
  return getInstance()->value("LastOpenedFileName", "").toString();
}

void Settings::setLastOpenedFile(const QString& fileName) {
  getInstance()->setValue("LastOpenedFileName", fileName);
}

bool Settings::getLastOpenedFileEnable() {
  return getInstance()->value("LastOpenedFileNameEnable", true).toBool();
}

void Settings::setLastOpenedFileEnable(const bool enable) {
  getInstance()->setValue("LastOpenedFileNameEnable", enable);
}

QString Settings::getBookBgColor() {
  return getInstance()->value("BookDesignBgColor", "#ffeeaa").toString();
}
void Settings::setBookBgColor(const QString& color) {
  getInstance()->setValue("BookDesignBgColor", color);
}

bool Settings::getCustomStyleEnabled() {
  return getInstance()->value("CustomStyleEnabled", false).toBool();
}
void Settings::setCustomStaticEnabled(const bool enable) {
  getInstance()->setValue("CustomStyleEnabled", enable);
}

QString Settings::getCustomStyleUrl() {
  return getInstance()->value("CustomStyleUrl").toString();
}
void Settings::setCustomStyleUrl(const QString& url) {
  getInstance()->setValue("CustomStyleUrl", url);
}
