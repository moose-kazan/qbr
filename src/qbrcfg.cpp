#include "qbrcfg.h"
#include <QSettings>

qbrcfg::qbrcfg()
{

}

QSettings* qbrcfg::cfg = nullptr;

QSettings* qbrcfg::getInstance()
{
    if (cfg == nullptr) {
        cfg = new QSettings("YLSoftware", "QBR");
    }
    return cfg;
}

bool qbrcfg::getStatusBarEnabled()
{
    return getInstance()->value("statusBarEnabled", true).toBool();
}
void qbrcfg::setStatusBarEnabled(bool enabled)
{
    getInstance()->setValue("statusBarEnabled", enabled);
}

int qbrcfg::getUiVariant()
{
    return getInstance()->value("uiVariant", 0).toInt();
}

void qbrcfg::setUiVariant(int uiVaraint)
{
    getInstance()->setValue("uiVariant", uiVaraint);
}

void qbrcfg::setFilePosition(QString fileName, QPointF pos)
{
    QString cfgkey = "filePos_" + fileName;
    getInstance()->setValue(cfgkey, pos);
}

QPointF qbrcfg::getFilePosition(QString fileName)
{
    QString cfgkey = "filePos_" + fileName;
    QSettings* cfg = getInstance();
    if (cfg->contains(cfgkey)) {
        return getInstance()->value(cfgkey).toPointF();
    }
    return QPointF(0, 0);
}

QByteArray qbrcfg::getMainWindowState(QByteArray state)
{
    QSettings* cfg = getInstance();
    if (cfg->contains("mainWindowState")) {
        return cfg->value("mainWindowState").toByteArray();
    }
    return state;
}

void qbrcfg::setMainWindowState(QByteArray state)
{
    getInstance()->setValue("mainWindowState", state);
}

QByteArray qbrcfg::getMainWindowGeometry(QByteArray geometry)
{
    QSettings* cfg = getInstance();
    if (cfg->contains("mainWindowGeometry")) {
        return cfg->value("mainWindowGeometry").toByteArray();
    }
    return geometry;
}

void qbrcfg::setMainWindowGeometry(QByteArray geometry)
{
    getInstance()->setValue("mainWindowGeometry", geometry);
}

QString qbrcfg::getLastOpenedFile() {
    return getInstance()->value("LastOpenedFileName", "").toString();
}

void qbrcfg::setLastOpenedFile(QString fileName) {
    getInstance()->setValue("LastOpenedFileName", fileName);
}

bool qbrcfg::getLastOpenedFileEnable() {
    return getInstance()->value("LastOpenedFileNameEnable", true).toBool();
}

void qbrcfg::setLastOpenedFileEnable(bool enable) {
    getInstance()->setValue("LastOpenedFileNameEnable", enable);
}

QString qbrcfg::getBookBgColor()
{
    return getInstance()->value("BookDesignBgColor", "#ffeeaa").toString();
}
void qbrcfg::setBookBgColor(QString color)
{
    getInstance()->setValue("BookDesignBgColor", color);
}

bool qbrcfg::getCustomStyleEnabled()
{
    return getInstance()->value("CustomStyleEnabled", false).toBool();
}
void qbrcfg::setCustomStaticEnabled(bool enable)
{
    getInstance()->setValue("CustomStyleEnabled", enable);
}

QString qbrcfg::getCustomStyleUrl()
{
    return getInstance()->value("CustomStyleUrl").toString();
}
void qbrcfg::setCustomStyleUrl(QString url)
{
    getInstance()->setValue("CustomStyleUrl", url);
}


