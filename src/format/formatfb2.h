#ifndef QBRFORMATFB2_H
#define QBRFORMATFB2_H

#include "format.h"
#include <QString>
#include <QDomNode>


class FormatFB2 final : public Format
{
public:
    FormatFB2();
    ~FormatFB2() override;

    FormatFB2(const FormatFB2&) = delete;
    FormatFB2& operator=(const FormatFB2&) = delete;

    bool loadFile(const QString& fileName, const QByteArray& fileData, const qbrunzip* zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook* getBook() override;
    bool needUnzip() override;
private:
    QBRBook *book;
    QDomNode parseXmlNode(const QDomNode& currentNode, const QHash<QString, QString>& fb2Binaries, int* tocIndex, QList<QBRTocItem>* tocList);
    bool parseXml(const QByteArray& fileData);
    void parseBookInfo(const QDomDocument *parserXml) const;
};

#endif // QBRFORMATFB2_H
