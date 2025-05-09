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
    QString getFormatTitle() override;
    QBRBook getBook() override;
    bool needUnzip() override;
private:
    QBRBookInfo bookInfo;
    QString htmlData;
    static QDomNode parseXmlNode(const QDomNode& currentNode, const QHash<QString, QString>& fb2Binaries);
    bool parseXml(const QByteArray& fileData);
    void parseBookInfo(const QDomDocument *parserXml);
};

#endif // QBRFORMATFB2_H
