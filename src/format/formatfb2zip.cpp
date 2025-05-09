#include "formatfb2zip.h"

FormatFB2Zip::FormatFB2Zip() {}

bool FormatFB2Zip::loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) {
    (void)fileName;

    bookInfo.metadata.clear();

    if (!isZipFile(fileData))  {
        return false;
    }

    if (!zipData->isLoaded()) {
        return false;
    }

    QStringList zipEntryNames = zipData->getFileNameList();
    for (int i = 0; i < zipEntryNames.count(); i++) {
        QString zipEntryName = zipEntryNames.at(i);
        QString mimeType = "";
        if (zipEntryName.endsWith(".fb2", Qt::CaseInsensitive)) {
            if (parserFB2.loadFile(zipEntryName, zipData->getFileData(zipEntryName), nullptr)) {
                bookInfo = parserFB2.getBook();
                bookInfo.metadata.FileFormat = getFormatTitle();
                return true;
            }
        }
    }


    return false;
}

QBRBook FormatFB2Zip::getBook() {
    return bookInfo;
}

QStringList FormatFB2Zip::getExtensions() {
    return QStringList("fb2.zip");
}

QString FormatFB2Zip::getFormatTitle()
{
    return "Zipped FictionBook 2";
}

bool FormatFB2Zip::needUnzip() {
    return true;
}
