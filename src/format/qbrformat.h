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

class QBRFormat
{
public:
    QBRFormat();
    virtual bool loadFile(QString fileName, QByteArray fileData) { return fileName != "" && fileData.size() > 0 && false; };
    virtual QString getHtml() { return QString(""); };
    virtual QStringList getExtensions() { return QStringList(); };
    virtual QBRBookInfo getBookInfo() { return QBRBookInfo{}; };
    bool isZipFile(QByteArray data);
};

#endif // QBRFORMAT_H
