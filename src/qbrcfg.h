#ifndef QBRCFG_H
#define QBRCFG_H

#include <QSettings>

class qbrcfg
{
public:
    qbrcfg();

    static const int uiToolbarAndMenu = 0;
    static const int uiMenuOnly = 1;
    static const int uiToolbarOnly = 2;

    static QSettings* getInstance();
    static bool getStatusBarEnabled();
    static void setStatusBarEnabled(bool enabled);
    static int getUiVariant();
    static void setUiVariant(int setUiVariant);

protected:
    static QSettings* cfg;
};

#endif // QBRCFG_H
