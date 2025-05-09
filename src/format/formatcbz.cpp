#include "formatcbz.h"
#include "../template.h"

#include <QByteArray>
#include <QList>
#include <QRegularExpression>
#include <QString>
#include <QtCore>

FormatCBZ::FormatCBZ() {}

QStringList FormatCBZ::getExtensions() { return QStringList("cbz"); }

QString FormatCBZ::getFormatTitle()
{
  return "Comics Book Zip";
}


bool FormatCBZ::loadFile(const QString fileName, const QByteArray fileData, qbrzip *zipData) {
  htmlData = ""; // reset data from previous file
  bookInfo.clear();
  bookInfo.FileFormat = getFormatTitle();

  if (!fileNameRegexp.match(fileName).hasMatch()) {
    return false;
  }

  if (!isZipFile(fileData)) {
    return false;
  }

  if (!zipData->isLoaded()) {
    return false;
  }

  QStringList zipEntryNames = zipData->getFileNameList();

  zipEntryNames.sort(Qt::CaseInsensitive);

  htmlData.append(Template::header());

  for (int i = 0; i < zipEntryNames.count(); i++) {
    QString zipEntryName = zipEntryNames.at(i);
    QString mimeType = "";
    if (zipEntryName.endsWith(".png", Qt::CaseInsensitive)) {
      mimeType = "image/png";
    } else if (zipEntryName.endsWith(".gif", Qt::CaseInsensitive)) {
      mimeType = "image/gif";
    } else if (zipEntryName.endsWith(".jpg", Qt::CaseInsensitive)) {
      mimeType = "image/jpeg";
    } else if (zipEntryName.endsWith(".jpeg", Qt::CaseInsensitive)) {
      mimeType = "image/jpeg";
    }

    if (mimeType != "") {
      QByteArray entryData = zipData->getFileData(zipEntryName);
      htmlData.append("<div class=\"comics_image\"><img src=\"data:");
      htmlData.append(mimeType);
      htmlData.append(";base64,");
      htmlData.append(entryData.toBase64());
      htmlData.append("\"><br /></div>\n");
      if (i == 0) {
          bookInfo.Cover.loadFromData(entryData);
      }
    }
  }

  htmlData.append(Template::footer());

  return true;
}

QBRBook FormatCBZ::getBook() { return QBRBook{bookInfo, htmlData}; }

bool FormatCBZ::needUnzip() { return true; }
