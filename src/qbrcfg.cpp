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
