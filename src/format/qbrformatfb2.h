#ifndef QBRFORMATFB2_H
#define QBRFORMATFB2_H

#include "../qbrformat.h"
#include <QString>
#include <QByteArray>
#include <QDomNode>


class qbrformatfb2 : public qbrformat
{
public:
    qbrformatfb2();
    bool loadFile(QString fileName, QByteArray fileData) override;
    QString getHtml() override;

private:
    QString htmlData;
    bool parseXml(QByteArray fileData);
    QString parseXmlTextFromNode(QDomNode xmlNode);
    QString parseXmlBody(QDomNode xmlNode, QHash<QString, QString> xmlImages);
};

#endif // QBRFORMATFB2_H
