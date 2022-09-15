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

QByteArray qbrcfg::getMainWindowState()
{
    return getInstance()->value("mainWindowState").toByteArray();
}

void qbrcfg::setMainWindowState(QByteArray state)
{
    getInstance()->setValue("mainWindowState", state);
}

QByteArray qbrcfg::getMainWindowGeometry()
{
    return getInstance()->value("mainWindowGeometry").toByteArray();
}

void qbrcfg::setMainWindowGeometry(QByteArray geometry)
{
    getInstance()->setValue("mainWindowGeometry", geometry);
}


