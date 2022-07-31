#ifndef QBRZIP_H
#define QBRZIP_H

#include <quazip5/quazip.h>
#include <quazip5/quazipfile.h>

#include <QByteArray>
#include <QStringList>
#include <QString>
#include <QHash>

class qbrzip
{
public:
    qbrzip(bool CS = true);
    bool setData(QByteArray zipData);
    QStringList getFileNameList();
    QByteArray getFileData(QString fileName);

private:
    bool entry_names_cs;
    QuaZip zipArchive;
    QHash<QString, QByteArray> zipEntries;
};

#endif // QBRZIP_H
