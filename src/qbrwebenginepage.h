#ifndef QBRWEBENGINEPAGE_H
#define QBRWEBENGINEPAGE_H

#include <QWebEnginePage>

class qbrWebEnginePage final : public QWebEnginePage
{
public:
    qbrWebEnginePage();
    void positionSave(const QString& fileName) const;
    void positionRestore(const QString& fileName);

protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
};

#endif // QBRWEBENGINEPAGE_H
