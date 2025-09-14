//
// Created by moose on 13.09.25.
//

#ifndef QBRZIP_H
#define QBRZIP_H
#include <QList>
#include <QString>
#include <zip.h>

struct zipItem
{
    QString name;
    QByteArray data;
    enum : int
    {
        METHOD_DEFAULT = ZIP_CM_DEFAULT,
        METHOD_DEFLATE = ZIP_CM_DEFLATE,
        METHOD_STORE = ZIP_CM_STORE,
        METHOD_BZIP2 = ZIP_CM_BZIP2,
        METHOD_XZ = ZIP_CM_XZ
    } method = METHOD_DEFAULT;
};

class qbrzip {
public:
    qbrzip();
    void newFile();
    void addItem(zipItem item);
    bool save(QString filename);

private:
    QList<zipItem> items;
};



#endif //QBRZIP_H
