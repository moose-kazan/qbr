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
    QDomNode parseXmlNode(QDomNode currentNode, QHash<QString, QString> fb2Binaries);
    bool parseXml(QByteArray fileData);
    void parseBookInfo(QDomDocument *parserXml);
};

#endif // QBRFORMATFB2_H
