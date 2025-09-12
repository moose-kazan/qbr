#ifndef QBRFORMATCBZ_H
#define QBRFORMATCBZ_H

#include "format.h"
#include "../libs/qbrzip.h"
#include <QRegularExpression>
#include <QString>


class FormatCBZ : public Format
{
public:
    FormatCBZ();
    bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook getBook() override;
    bool needUnzip() override;
private:
    QBRBookInfo bookInfo;
    QString htmlData;
    QRegularExpression fileNameRegexp = QRegularExpression("\\.cbz$", QRegularExpression::CaseInsensitiveOption);
};

#endif // QBRFORMATCBZ_H
