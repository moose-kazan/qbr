#include "formatcbz.h"
#include "../template.h"

#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QList>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QtCore>

FormatCBZ::FormatCBZ() {}

QStringList FormatCBZ::getExtensions() { return QStringList("cbz"); }

bool FormatCBZ::loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) {
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

  unZip = zipData;

  if (!unZip->isLoaded()) {
    return false;
  }

  QStringList zipEntryNames = unZip->getFileNameList();

  zipEntryNames.sort(Qt::CaseInsensitive);

  htmlData.append(Template::header());

  for (int i = 0; i < zipEntryNames.count(); i++) {
    QString zipEntryName = zipEntryNames.at(i);
    QString mimeType = "";
    if (QRegularExpression("\\.png$", QRegularExpression::CaseInsensitiveOption)
            .match(zipEntryName)
            .hasMatch()) {
      mimeType = "image/png";
    } else if (QRegularExpression("\\.gif$",
                                  QRegularExpression::CaseInsensitiveOption)
                   .match(zipEntryName)
                   .hasMatch()) {
      mimeType = "image/gif";
    } else if (QRegularExpression("\\.jpe?g$",
                                  QRegularExpression::CaseInsensitiveOption)
                   .match(zipEntryName)
                   .hasMatch()) {
      mimeType = "image/jpeg";
    }

    if (mimeType != "") {
      htmlData.append("<div class=\"comics_image\"><img src=\"data:");
      htmlData.append(mimeType);
      htmlData.append(";base64,");
      htmlData.append(unZip->getFileData(zipEntryName).toBase64());
      htmlData.append("\"><br /></div>\n");
    }
  }

  htmlData.append(Template::footer());

  return true;
}

QBRBook FormatCBZ::getBook() { return QBRBook{bookInfo, htmlData}; }

bool FormatCBZ::needUnzip() { return true; }
