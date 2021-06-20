#ifndef QBRFORMAT_H
#define QBRFORMAT_H

#include <QString>
#include <QByteArray>

class qbrformat
{
public:
    qbrformat();
    virtual bool loadFile(QString fileName, QByteArray fileData) { return fileName != "" && fileData.size() > 0 && false; };
    virtual QString getHtml() { return QString(""); };
};

#endif // QBRFORMAT_H
