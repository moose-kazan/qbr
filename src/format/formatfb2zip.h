#ifndef QBRFORMATFB2ZIP_H
#define QBRFORMATFB2ZIP_H

#include "format.h"
#include "formatfb2.h"

class FormatFB2Zip final : public Format
{
public:
    FormatFB2Zip();
    bool loadFile(QString fileName, QByteArray fileData, qbrunzip *zipData) override;
    QBRBook getBook() override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    bool needUnzip() override;

private:
    QBRBook bookInfo;
    FormatFB2 parserFB2;
};

#endif // QBRFORMATFB2ZIP_H
