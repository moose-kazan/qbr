#ifndef QBRUNZIP_H
#define QBRUNZIP_H

#include <QHash>

class qbrunzip
{
public:
    qbrunzip(bool CS = true);
    bool setData(QByteArray zipData);
    QStringList getFileNameList() const;
    QByteArray getFileData(QString fileName) const;
    bool fileExists(QString fileName) const;
    void clear();
    bool isLoaded() const;

    const int MAX_FILE_NAME_LENGTH=256;

private:
    bool entry_names_cs;
    bool loaded;
    QHash<QString, QByteArray> zipEntries;
};

#endif // QBRUNZIP_H
