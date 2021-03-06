#include "qbrformatcbz.h"
#include "../qbrtemplate.h"
#include "../qbrzip.h"

#include <QString>
#include <QByteArray>
#include <QRegExp>
#include <QBuffer>
#include <QtCore>
#include <QList>
#include <QStringList>
#include <QDebug>
#include <QFile>

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

    if (!isZipFile(fileData))
    {
        return false;
    }

    qbrzip unZip;
    if (!unZip.setData(fileData))
    {
        return false;
    }

    QStringList zipEntryNames = unZip.getFileNameList();

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
            htmlData.append("<div class=\"comics_image\"><img src=\"data:");
            htmlData.append(mimeType);
            htmlData.append(";base64,");
            htmlData.append(unZip.getFileData(zipEntryName).toBase64());
            htmlData.append("\"><br /></div>\n");
        }
    }

    htmlData.append(qbrtemplate::footer());

    return true;
}

QString qbrformatcbz::getHtml()
{
    return htmlData;
}
