#include "qbrformatcbz.h"
#include "../qbrtemplate.h"

#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QList>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QtCore>

QBRFormatCBZ::QBRFormatCBZ() : unZip(false) {}

QStringList QBRFormatCBZ::getExtensions() { return QStringList("cbz"); }

bool QBRFormatCBZ::loadFile(QString fileName, QByteArray fileData) {
  htmlData = ""; // reset data from previous file
  bookInfo = {};
  bookInfo.FileFormat = "Comics Book Zip";

  QRegularExpression rx("\\.cbz$", QRegularExpression::CaseInsensitiveOption);
  if (!rx.match(fileName).hasMatch()) {
    return false;
  }

  if (!isZipFile(fileData)) {
    return false;
  }

  if (!unZip.setData(fileData)) {
    return false;
  }

  QStringList zipEntryNames = unZip.getFileNameList();

  zipEntryNames.sort(Qt::CaseInsensitive);

  htmlData.append(qbrtemplate::header());

  for (int i = 0; i < zipEntryNames.count(); i++) {
    QString zipEntryName = zipEntryNames.at(i);
    QString mimeType = "";
    if (QRegularExpression("\\.png$", QRegularExpression::CaseInsensitiveOption).match(zipEntryName).hasMatch()) {
      mimeType = "image/png";
    } else if (QRegularExpression("\\.gif$", QRegularExpression::CaseInsensitiveOption).match(zipEntryName).hasMatch()) {
      mimeType = "image/gif";
    } else if (QRegularExpression("\\.jpe?g$", QRegularExpression::CaseInsensitiveOption)
                   .match(zipEntryName).hasMatch()) {
      mimeType = "image/jpeg";
    }

    if (mimeType != "") {
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

QBRBook QBRFormatCBZ::getBook()
{
    return QBRBook{bookInfo, htmlData};
}
