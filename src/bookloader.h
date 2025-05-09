#ifndef BOOKLOADER_H
#define BOOKLOADER_H

#include <QRegularExpression>

#include "libs/qbrzip.h"
#include "format/format.h"
#include "format/formatamb.h"
#include "format/formatcbz.h"
#include "format/formatfb2.h"
#include "format/formatfb2zip.h"
#include "format/formatfb3.h"
#include "format/formatepub.h"

class BookLoader : public QObject
{
    Q_OBJECT
public:
    BookLoader();
    QBRBook getBook();
    QStringList getFilter();
    bool loadFile(const QString& fileName);
private:
    QList<Format*> bookParsers = {
        new FormatAMB(),
        new FormatCBZ(),
        new FormatEPub(),
        new FormatFB2(),
        new FormatFB2Zip(),
        new FormatFB3()
    };

    QBRBook bookData;
    qbrzip *unZip;
    QStringList filtersData;
    QRegularExpression re = QRegularExpression("^");
};

#endif // BOOKLOADER_H
