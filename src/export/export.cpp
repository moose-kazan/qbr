#include "export.h"

#include <QFile>

Export::Export() = default;

QString Export::getFilter()
{
    return "";
}

void Export::setData(QBRBook *book)
{
    (void)book;
}

bool Export::save(const QString fileName)
{
    (void)fileName;
    return false;
}

bool Export::_save(const QString& fileName, const QByteArray& data)
{
    try
    {
        if (QFile f(fileName); f.open(QIODevice::WriteOnly))
        {
            f.write(data);
            f.close();
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}
