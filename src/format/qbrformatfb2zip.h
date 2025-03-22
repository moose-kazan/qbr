#ifndef QBRFORMATFB2ZIP_H
#define QBRFORMATFB2ZIP_H

#include "qbrformat.h"
#include "qbrformatfb2.h"

class QBRFormatFB2Zip : public QBRFormat
{
public:
    QBRFormatFB2Zip();
    bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) override;
    QBRBook getBook() override;
    QStringList getExtensions() override;
    bool needUnzip() override;

private:
    QBRBook bookInfo;
    QBRFormatFB2 parserFB2;
};

#endif // QBRFORMATFB2ZIP_H
