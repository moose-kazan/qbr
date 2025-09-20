#include "qbrzip.h"
#include "zip.h"

#include <utility>
#include <QDebug>

qbrzip::qbrzip() = default;

void qbrzip::newFile()
{
    items.clear();
}

void qbrzip::addItem(const zipItem item)
{
    items.append(item);
}

bool qbrzip::save(QString filename)
{
    int zipError = 0;
    zip_t* zipFileHandle = zip_open(filename.toStdString().c_str(), ZIP_CREATE | ZIP_TRUNCATE, &zipError);
    if (zipError != 0)
    {
        qDebug() << "Error creating zip file";
        return false;
    }
    for (auto & [name, data, method] : items)
    {
        zip_source *source = zip_source_buffer(zipFileHandle, data.data(), data.length(), 0);
        if (source == nullptr)
        {
            qDebug() << "Error creating zip source";
            zip_close(zipFileHandle);
            return false;
        }
        const long entryIndex = zip_file_add(zipFileHandle, name.toStdString().c_str(), source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);
        if (entryIndex < 0)
        {
            qDebug() << "Error adding file to zip";
            zip_source_free(source);
            zip_close(zipFileHandle);
            return false;
        }
        if (const int result = zip_set_file_compression(zipFileHandle, entryIndex, method, 0); result < 0 )
        {
            qDebug() << "Error setting compression";
            zip_close(zipFileHandle);
            return false;
        }
    }

    zip_close(zipFileHandle);

    return true;
}