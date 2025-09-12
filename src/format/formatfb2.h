#ifndef QBRFORMATFB2_H
#define QBRFORMATFB2_H

#include "format.h"
#include <QString>
#include <QDomNode>


class FormatFB2 : public Format
{
public:
    FormatFB2();
    bool loadFile(QString fileName, QByteArray fileData, qbrunzip *zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook getBook() override;
    bool needUnzip() override;
private:
    qbrbookinfo bookInfo;
    QString htmlData;
    static QDomNode parseXmlNode(const QDomNode& currentNode, const QHash<QString, QString>& fb2Binaries);
    bool parseXml(const QByteArray& fileData);
    void parseBookInfo(const QDomDocument *parserXml);
};

#endif // QBRFORMATFB2_H
