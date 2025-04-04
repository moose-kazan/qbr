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
    QBRBookInfo bookInfo;
    QString htmlData;
    bool parseFile();
    QString parseFB3Node(QDomNode xmlNode);
    QString parseFB3TextFromNode(QDomNode xmlNode);
    qbrzip *unZip;
    QHash<QString, QString> fb3_binaries;
    void parseFB3Metadata(QString entryName);
};

#endif // QBRFORMATFB3_H
