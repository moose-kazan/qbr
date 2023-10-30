#include "qbrwebenginepage.h"
#include "qbrcfg.h"

#include <QDebug>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QUrl>
#include <QWebEnginePage>

qbrWebEnginePage::qbrWebEnginePage() : QWebEnginePage() {
  // qDebug() << "WebEnginePage created";
  QDesktopWidget desktop;

  int desktopDpi = desktop.logicalDpiY();

  setZoomFactor(desktopDpi > 72 ? desktopDpi / 72 : 1);
}

bool qbrWebEnginePage::acceptNavigationRequest(
    const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame) {
  (void)type;
  (void)isMainFrame;

  if (url.scheme() != "file" && url.scheme() != "data" &&
      url.scheme() != "javascript") {
    QDesktopServices::openUrl(url);
    return false;
  }
  return true;
}

void qbrWebEnginePage::positionSave(QString fileName) {
  QPointF sPos = scrollPosition();
  QSizeF sSize = contentsSize();
  QPointF pos = QPointF(sPos.x() / sSize.height(), sPos.y() / sSize.width());
  qbrcfg::setFilePosition(fileName, pos);
}

void qbrWebEnginePage::positionRestore(QString fileName) {
  qDebug() << "Restoring position for file: " << fileName;
  QPointF pos = qbrcfg::getFilePosition(fileName);
  runJavaScript(QString("window.scrollTo(%1*document.body.scrollHeight, "
                        "%2*document.body.scrollWidth);")
                    .arg(pos.x())
                    .arg(pos.y()));
}
