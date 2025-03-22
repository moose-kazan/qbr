#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QPointF>

class Settings
{
public:
    Settings();

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

    static QByteArray getMainWindowState(QByteArray state);
    static void setMainWindowState(QByteArray state);

    static QByteArray getMainWindowGeometry(QByteArray geometry);
    static void setMainWindowGeometry(QByteArray geometry);

    static QString getLastOpenedFile();
    static void setLastOpenedFile(QString fileName);

    static bool getLastOpenedFileEnable();
    static void setLastOpenedFileEnable(bool enable);

    static QString getBookBgColor();
    static void setBookBgColor(QString color);

    static bool getCustomStyleEnabled();
    static void setCustomStaticEnabled(bool enable);

    static QString getCustomStyleUrl();
    static void setCustomStyleUrl(QString url);

protected:
    static QSettings* cfg;
};

#endif // SETTINGS_H
