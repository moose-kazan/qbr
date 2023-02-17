#ifndef QBRCFG_H
#define QBRCFG_H

#include <QSettings>
#include <QPointF>

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

    static void setFilePosition(QString fileName, QPointF pos);
    static QPointF getFilePosition(QString fileName);

    static QByteArray getMainWindowState();
    static void setMainWindowState(QByteArray state);

    static QByteArray getMainWindowGeometry();
    static void setMainWindowGeometry(QByteArray geometry);

    static QString getLastOpenedFile();
    static void setLastOpenedFile(QString fileName);

    static bool getLastOpenedFileEnable();
    static void setLastOpenedFileEnable(bool enable);

protected:
    static QSettings* cfg;
};

#endif // QBRCFG_H
