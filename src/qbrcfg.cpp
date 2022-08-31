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

