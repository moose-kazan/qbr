#ifndef QBRBOOKLOADER_H
#define QBRBOOKLOADER_H

#include "libs/qbrzip.h"
#include "format/qbrformat.h"
#include "format/qbrformatamb.h"
#include "format/qbrformatcbz.h"
#include "format/qbrformatfb2.h"
#include "format/qbrformatfb3.h"

class QBRBookLoader
{
public:
    QBRBookLoader();
    QBRBook getBook();
    QStringList getExtensions();
    bool loadFile(QString fileName);
private:
    QList<QBRFormat*> bookParsers = {
        new QBRFormatAMB(),
        new QBRFormatCBZ(),
        new QBRFormatFB2(),
        new QBRFormatFB3()
    };

    QBRBook bookData;
    qbrzip *unZip;
};

#endif // QBRBOOKLOADER_H
