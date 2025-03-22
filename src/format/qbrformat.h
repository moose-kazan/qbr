#ifndef QBRFORMAT_H
#define QBRFORMAT_H

#include "../libs/qbrzip.h"

#include <QString>
#include <QByteArray>
#include <QStringList>

struct QBRBookInfo {
    QString Author;
    QString Title;
    QString FileFormat;
};

struct QBRBook {
    QBRBookInfo metadata;
    QString html;
};

class QBRFormat
{
public:
    QBRFormat();
    virtual bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData);
    virtual QBRBook getBook();
    virtual QStringList getExtensions();
    virtual bool needUnzip();
    static bool isZipFile(QByteArray data);
};

#endif // QBRFORMAT_H
