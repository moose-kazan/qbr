#ifndef QBRFORMAT_H
#define QBRFORMAT_H

#include <QString>

class qbrformat
{
public:
    qbrformat();
    virtual bool loadFile(QString fileName) { return fileName != "" && false; };
    virtual QString getHtml() { return QString(""); };
};

#endif // QBRFORMAT_H
