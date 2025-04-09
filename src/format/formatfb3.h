#ifndef QBRFORMATFB3_H
#define QBRFORMATFB3_H

#include "../template.h"
#include "format.h"
#include "../libs/qbrzip.h"
#include <QString>
#include <QByteArray>
#include <QDomNode>
#include <QHash>


class FormatFB3 : public Format
{
public:
    FormatFB3();
    bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) override;
    QStringList getExtensions() override;
    QBRBook getBook() override;
    bool needUnzip() override;
private:
    const QString RELATION_THUMBNAIL      = "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail";
    const QString RELATION_COREPROPERTIES = "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties";
    const QString RELATION_BOOK           = "http://www.fictionbook.org/FictionBook3/relationships/Book";

    QBRBookInfo bookInfo;
    QString htmlData;
    QString expandFileName(QString baseFileName, QString expandableFileName);
    QString getRelsFileName(QString baseFileName);
    void parseMetadata(qbrzip *ZipData, QString entryName);
    QList<QDomElement> parseRels(qbrzip *ZipData, QString entryName);
    QDomNode parseBodyNode(QDomNode currentNode, QMap<QString,QString> bodyBinaries);
    QString parseBody(qbrzip *zipData, QString bodyEntryName);
    bool parseFile(qbrzip *ZipData);
    QHash<QString, QString> fb3ExtTypes;

};

#endif // QBRFORMATFB3_H
