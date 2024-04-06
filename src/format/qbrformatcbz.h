#ifndef QBRFORMATCBZ_H
#define QBRFORMATCBZ_H

#include "qbrformat.h"
#include "../libs/qbrzip.h"
#include <QByteArray>
#include <QString>


class QBRFormatCBZ : public QBRFormat
{
public:
    QBRFormatCBZ();
    bool loadFile(QString fileName, QByteArray fileData) override;
    QStringList getExtensions() override;
    QBRBook getBook() override;
private:
    QBRBookInfo bookInfo;
    QString htmlData;
    qbrzip unZip;
};

#endif // QBRFORMATCBZ_H
