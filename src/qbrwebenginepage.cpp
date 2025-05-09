#include "qbrwebenginepage.h"
#include "libs/settings.h"
#include "libs/helpers.h"

#include <QDebug>
#include <QDesktopServices>
#include <QPointF>
#include <QSizeF>
#include <QUrl>

qbrWebEnginePage::qbrWebEnginePage() : QWebEnginePage() {
  // qDebug() << "WebEnginePage created";

  setZoomFactor(Helpers::getDesktopScale());
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

void qbrWebEnginePage::positionSave(const QString& fileName) const
{
  QPointF sPos = scrollPosition();
  QSizeF sSize = contentsSize();
  QPointF pos = QPointF(sPos.x() / sSize.height(), sPos.y() / sSize.width());
  Settings::setFilePosition(fileName, pos);
}

void qbrWebEnginePage::positionRestore(QString fileName) {
  qDebug() << "Restoring position for file: " << fileName;
  const QPointF pos = Settings::getFilePosition(fileName);
  runJavaScript(QString("window.scrollTo(%1*document.body.scrollHeight, "
                        "%2*document.body.scrollWidth);")
                    .arg(pos.x())
                    .arg(pos.y()));
}
