#ifndef QBRFORMATCBZ_H
#define QBRFORMATCBZ_H

#include "format.h"
#include "../libs/qbrunzip.h"
#include <QRegularExpression>
#include <QString>


class FormatCBZ final : public Format
{
public:
    FormatCBZ();
    bool loadFile(QString fileName, QByteArray fileData, qbrunzip *zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook getBook() override;
    bool needUnzip() override;
private:
    qbrbookinfo bookInfo;
    QString htmlData;
    QRegularExpression fileNameRegexp = QRegularExpression("\\.cbz$", QRegularExpression::CaseInsensitiveOption);
};

#endif // QBRFORMATCBZ_H
