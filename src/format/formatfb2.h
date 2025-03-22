#ifndef QBRFORMATFB2_H
#define QBRFORMATFB2_H

#include "format.h"
#include <QString>
#include <QByteArray>
#include <QDomNode>


class FormatFB2 : public Format
{
public:
    FormatFB2();
    bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) override;
    QStringList getExtensions() override;
    QBRBook getBook() override;
    bool needUnzip() override;
private:
    QBRBookInfo bookInfo;
    QString htmlData;
    bool parseXml(QByteArray fileData);
    QString parseXmlTextFromNode(QDomNode xmlNode);
    QString parseXmlBody(QDomNode xmlNode, QHash<QString, QString> xmlImages);
    void parseBookInfo(QDomDocument *parserXml);
};

#endif // QBRFORMATFB2_H
