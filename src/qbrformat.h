#ifndef QBRFORMAT_H
#define QBRFORMAT_H

#include <QString>
#include <QByteArray>
#include <QStringList>

class QBRFormat
{
public:
    QBRFormat();
    virtual bool loadFile(QString fileName, QByteArray fileData) { return fileName != "" && fileData.size() > 0 && false; };
    virtual QString getHtml() { return QString(""); };
    virtual QStringList getExtensions() { return QStringList(); };
    bool isZipFile(QByteArray data);
};

#endif // QBRFORMAT_H
