#ifndef FORMAT_H
#define FORMAT_H

#include "../libs/qbrzip.h"

#include <QObject>
#include <QString>
#include <QImage>

struct QBRBookInfo {
    QString Author;
    QString Title;
    QString Description;
    QString FileFormat;
    QImage Cover;
    void clear() {
        Author = "";
        Title = "";
        Description = "";
        FileFormat = "";
        Cover = QImage();
    }
};

struct QBRBook {
    QBRBookInfo metadata;
    QString html;
};

class Format : public QObject
{
    Q_OBJECT
public:
    Format();
    virtual bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData);
    virtual QBRBook getBook();
    virtual QStringList getExtensions();
    virtual QString getFormatTitle();
    virtual bool needUnzip();
    static bool isZipFile(const QByteArray& data);
};

#endif // FORMAT_H
