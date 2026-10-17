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
    ~FormatCBZ() override;

    FormatCBZ(const FormatCBZ&) = delete;
    FormatCBZ& operator=(const FormatCBZ&) = delete;

    bool loadFile(const QString& fileName, const QByteArray& fileData, const qbrunzip* zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook* getBook() override;
    bool needUnzip() override;
private:
    QBRBook *book;
    QRegularExpression fileNameRegexp = QRegularExpression("\\.cbz$", QRegularExpression::CaseInsensitiveOption);
};

#endif // QBRFORMATCBZ_H
