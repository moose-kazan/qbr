#ifndef QBRZIP_H
#define QBRZIP_H

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
    void clear();

    const int MAX_FILE_NAME_LENGTH=256;

private:
    bool entry_names_cs;
    QHash<QString, QByteArray> zipEntries;
};

#endif // QBRZIP_H
