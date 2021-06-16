#include "qbrzip.h"
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

#include <QByteArray>
#include <QBuffer>
#include <QDebug>
#include <QStringList>

// Based on https://gist.github.com/mobius/1759816
// Some ideas found here: https://github.com/ctabin/libzippp/blob/master/src/


qbrzip::qbrzip()
{

}

bool qbrzip::setData(QByteArray zipData)
{
    QBuffer* buff = new QBuffer(&zipData);
    zipArchive.setIoDevice(buff);
    zipArchive.open(QuaZip::mdUnzip);
    if (zipArchive.getZipError() != UNZ_OK)
    {
        return false;
    }

    QuaZipFile zipFile(&zipArchive);

    for(bool more = zipArchive.goToFirstFile(); more; more = zipArchive.goToNextFile())
    {
        QString zipEntryName = zipArchive.getCurrentFileName();
        QByteArray zipEntryData;
        if (zipEntryName.at(zipEntryName.length() - 1) != "/")
        {
            if (!zipFile.open(QIODevice::ReadOnly))
            {
                return false;
            }
            int buff_size = 4096;
            char buff[buff_size];
            while (!zipFile.atEnd())
            {

                int readed = zipFile.read(buff, buff_size);
                if (readed > 0)
                {
                    zipEntryData.append(buff, readed);
                }
            }
            zipFile.close();
            zipEntries.insert(zipEntryName, zipEntryData);
        }

    }
    return true;

}

QStringList qbrzip::getFileNameList()
{
    QStringList rv(zipEntries.keys());
    return rv;
}

QByteArray qbrzip::getFileData(QString fileName)
{
    return zipEntries.value(fileName, NULL);
}
