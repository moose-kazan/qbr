#include "qbrwebenginepage.h"
#include "qbrcfg.h"

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
    (void)type;
    (void)isMainFrame;

    if (url.scheme() != "file" && url.scheme() != "data" && url.scheme() != "javascript")
    {
        QDesktopServices::openUrl(url);
        return false;
    }
    return true;
}

void qbrWebEnginePage::positionSave(QString fileName)
{
    QSettings* cfg = qbrcfg::getInstance();
    QPointF sPos = scrollPosition();
    QSizeF sSize = contentsSize();
    QPointF pos = QPointF(
                    sPos.x()/sSize.height(),
                    sPos.y()/sSize.width()
                );
    cfg->setValue(fileName, pos);
}

void qbrWebEnginePage::positionRestore(QString fileName)
{
    qDebug() << "Restoring position for file: " << fileName;
    QSettings* cfg = qbrcfg::getInstance();
    if (cfg->contains(fileName))
    {
        QPointF pos = cfg->value(fileName).toPointF();
        runJavaScript(QString("window.scrollTo(%1*document.body.scrollHeight, %2*document.body.scrollWidth);").arg(pos.x()).arg(pos.y()));
    }
}

