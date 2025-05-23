#ifndef QBRWEBENGINEPAGE_H
#define QBRWEBENGINEPAGE_H

#include <QWebEnginePage>
#include <QUrl>

class qbrWebEnginePage : public QWebEnginePage
{
public:
    qbrWebEnginePage();
    void positionSave(const QString& fileName) const;
    void positionRestore(QString fileName);

protected:
    bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);
};

#endif // QBRWEBENGINEPAGE_H
