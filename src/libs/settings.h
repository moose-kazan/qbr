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

    static void setFilePosition(const QString& fileName, QPointF pos);
    static QPointF getFilePosition(const QString& fileName);

    static QByteArray getMainWindowState(QByteArray state);
    static void setMainWindowState(const QByteArray& state);

    static QByteArray getMainWindowGeometry(QByteArray geometry);
    static void setMainWindowGeometry(const QByteArray& geometry);

    static QString getLastOpenedFile();
    static void setLastOpenedFile(const QString& fileName);

    static bool getLastOpenedFileEnable();
    static void setLastOpenedFileEnable(bool enable);

    static QString getBookBgColor();
    static void setBookBgColor(const QString& color);

    static bool getCustomStyleEnabled();
    static void setCustomStaticEnabled(bool enable);

    static QString getCustomStyleUrl();
    static void setCustomStyleUrl(const QString& url);

protected:
    static QSettings* cfg;
};

#endif // SETTINGS_H
