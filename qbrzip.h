#ifndef QBRZIP_H
#define QBRZIP_H

#include <quazip/quazip.h>

#include <QByteArray>
#include <QStringList>
#include <QString>
#include <QHash>

class qbrzip
{
public:
    qbrzip();
    bool setData(QByteArray zipData);
    QStringList getFileNameList();
    QByteArray getFileData(QString fileName);

private:
    QuaZip zipArchive;
    QHash<QString, QByteArray> zipEntries;
};

#endif // QBRZIP_H
