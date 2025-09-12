#ifndef QBRFORMATFB3_H
#define QBRFORMATFB3_H

#include "format.h"
#include "../libs/qbrzip.h"
#include <QString>
#include <QDomNode>
#include <QHash>
#include <QRegularExpression>


class FormatFB3 : public Format
{
public:
    FormatFB3();
    bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook getBook() override;
    bool needUnzip() override;
private:
    const QString RELATION_THUMBNAIL      = "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail";
    const QString RELATION_COREPROPERTIES = "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties";
    const QString RELATION_BOOK           = "http://www.fictionbook.org/FictionBook3/relationships/Book";

    QRegularExpression fileNameRegexp = QRegularExpression("\\.fb3$", QRegularExpression::CaseInsensitiveOption);
    QBRBookInfo bookInfo;
    QString htmlData;
    static QString expandFileName(const QString& baseFileName, QString expandableFileName);
    static QString getRelsFileName(const QString& baseFileName);
    void parseMetadata(const qbrzip *ZipData, const QString& entryName);
    static QList<QDomElement> parseRels(const qbrzip *ZipData, const QString& entryName);
    static QDomNode parseBodyNode(const QDomNode& currentNode, const QMap<QString,QString>& bodyBinaries);
    QList<QDomNode> parseBody(const qbrzip* zipData, const QString& bodyEntryName) const;
    bool parseFile(const qbrzip *ZipData);
    QHash<QString, QString> fb3ExtTypes;

};

#endif // QBRFORMATFB3_H
