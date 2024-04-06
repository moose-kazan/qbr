#ifndef QBRFORMAT_H
#define QBRFORMAT_H

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
    virtual bool loadFile(QString fileName, QByteArray fileData) { return fileName != "" && fileData.size() > 0 && false; };
    virtual QBRBook getBook() { return QBRBook{}; };
    virtual QStringList getExtensions() { return QStringList(); };
    bool isZipFile(QByteArray data);
};

#endif // QBRFORMAT_H
