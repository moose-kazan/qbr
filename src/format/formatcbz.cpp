#include "formatcbz.h"

#include <QByteArray>
#include <QList>
#include <QRegularExpression>
#include <QString>

FormatCBZ::FormatCBZ() = default;

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

  templateInit();

  for (int i = 0; i < zipEntryNames.count(); i++) {
    const QString& zipEntryName = zipEntryNames.at(i);
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
      QDomElement nodeDiv = templateCreateElement("div");
      nodeDiv.setAttribute("class", "comics_image");

      QByteArray entryData = zipData->getFileData(zipEntryName);
      QString imgData = "data:" + mimeType + ";base64," + entryData.toBase64();
      QDomElement nodeImg = templateCreateElement("img");
      nodeImg.setAttribute("src", imgData);
      nodeImg.setAttribute("id", zipEntryName);

      nodeDiv.appendChild(nodeImg);
      nodeDiv.appendChild(templateCreateElement("br"));
      templateBodyAppend(nodeDiv);

      if (i == 0) {
          bookInfo.Cover.loadFromData(entryData);
      }
    }
  }

  htmlData = templateAsString();
  return true;
}

QBRBook FormatCBZ::getBook() { return QBRBook{bookInfo, htmlData}; }

bool FormatCBZ::needUnzip() { return true; }
