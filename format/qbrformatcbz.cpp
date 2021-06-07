#include "qbrformatcbz.h"
#include "../qbrtemplate.h"

#include <QString>
#include <QByteArray>
#include <QRegExp>
#include <QBuffer>
#include <QtCore>
#include <QList>
#include <QStringList>
#include <QDebug>

#include <private/qzipreader_p.h>

qbrformatcbz::qbrformatcbz()
{

}

bool qbrformatcbz::loadFile(QString fileName, QByteArray fileData)
{
    QRegExp rx("\\.cbz$", Qt::CaseInsensitive);
    if (rx.indexIn(fileName) < 0)
    {
        return false;
    }

    QBuffer* buff = new QBuffer();
    buff->setData(fileData);
    QZipReader* zip = new QZipReader(buff);

    QStringList zipEntryNames;

    for (int i = 0; i < zip->count(); i++)
    {
        if (zip->entryInfoAt(i).isFile == 1)
        {
            zipEntryNames.append(zip->entryInfoAt(i).filePath);
        }
    }

    zipEntryNames.sort(Qt::CaseInsensitive);

    htmlData.append(qbrtemplate::header());

    for (int i = 0; i < zipEntryNames.count(); i++)
    {
        QString zipEntryName = zipEntryNames.at(i);
        QString mimeType = "";
        if (QRegExp("\\.png$", Qt::CaseInsensitive).indexIn(zipEntryName))
        {
            mimeType = "image/png";
        }
        else if (QRegExp("\\.gif$", Qt::CaseInsensitive).indexIn(zipEntryName))
        {
            mimeType = "image/gif";
        }
        else if (QRegExp("\\.jpe?g$", Qt::CaseInsensitive).indexIn(zipEntryName))
        {
            mimeType = "image/jpeg";
        }

        if (mimeType != "")
        {
            htmlData.append("<img class=\"comics_image\" src=\"data:");
            htmlData.append(mimeType);
            htmlData.append(";base64,");
            htmlData.append(zip->fileData(zipEntryName).toBase64());
            htmlData.append("\"><br />\n");
        }
    }

    htmlData.append(qbrtemplate::footer());

    return true;
}

QString qbrformatcbz::getHtml()
{
    qDebug() << htmlData;
    return htmlData;
}
