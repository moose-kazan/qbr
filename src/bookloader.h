#ifndef BOOKLOADER_H
#define BOOKLOADER_H

#include "libs/qbrzip.h"
#include "format/format.h"
#include "format/formatamb.h"
#include "format/formatcbz.h"
#include "format/formatfb2.h"
#include "format/formatfb2zip.h"
#include "format/formatfb3.h"

class BookLoader
{
public:
    BookLoader();
    QBRBook getBook();
    QStringList getExtensions();
    bool loadFile(QString fileName);
private:
    QList<Format*> bookParsers = {
        new FormatAMB(),
        new FormatCBZ(),
        new FormatFB2(),
        new FormatFB2Zip(),
        new FormatFB3()
    };

    QBRBook bookData;
    qbrzip *unZip;
};

#endif // BOOKLOADER_H
