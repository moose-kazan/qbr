#include "qbrwebenginepage.h"

#include <QWebEnginePage>
#include <QDebug>
#include <QUrl>
#include <QDesktopServices>

qbrWebEnginePage::qbrWebEnginePage() : QWebEnginePage()
{
    //qDebug() << "WebEnginePage created";
}


bool qbrWebEnginePage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
    //qDebug() << url;
    if (url.scheme() != "file" && url.scheme() != "data")
    {
        QDesktopServices::openUrl(url);
        return false;
    }
    return true;
}
