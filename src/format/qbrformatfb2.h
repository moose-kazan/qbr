#ifndef QBRFORMATFB2_H
#define QBRFORMATFB2_H

#include "qbrformat.h"
#include <QString>
#include <QByteArray>
#include <QDomNode>


class QBRFormatFB2 : public QBRFormat
{
public:
    QBRFormatFB2();
    bool loadFile(QString fileName, QByteArray fileData) override;
    QStringList getExtensions() override;
    QString getHtml() override;
    QBRBookInfo getBookInfo() override;
private:
    QDomDocument *parserXml;
    QBRBookInfo bookInfo;
    QString htmlData;
    bool parseXml(QByteArray fileData);
    QString parseXmlTextFromNode(QDomNode xmlNode);
    QString parseXmlBody(QDomNode xmlNode, QHash<QString, QString> xmlImages);
    void parseBookInfo(QDomDocument *parserXml);
};

#endif // QBRFORMATFB2_H
