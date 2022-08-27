#ifndef QBRCFG_H
#define QBRCFG_H

#include <QSettings>

class qbrcfg
{
public:
    qbrcfg();

    static QSettings* getInstance();

protected:
    static QSettings* cfg;
};

#endif // QBRCFG_H
