#include "qbrformatfb2zip.h"

#include <QRegularExpression>

QBRFormatFB2Zip::QBRFormatFB2Zip() {}

bool QBRFormatFB2Zip::loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) {
    (void)fileName;

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
        if (QRegularExpression("\\.fb2$", QRegularExpression::CaseInsensitiveOption)
                .match(zipEntryName)
                .hasMatch()) {
            if (parserFB2.loadFile(zipEntryName, zipData->getFileData(zipEntryName), NULL)) {
                bookInfo = parserFB2.getBook();
                bookInfo.metadata.FileFormat = "Zipped FictionBook 2";
                return true;
            }
        }
    }


    return false;
}

QBRBook QBRFormatFB2Zip::getBook() {
    return bookInfo;
}

QStringList QBRFormatFB2Zip::getExtensions() {
    return QStringList("fb2.zip");
}

bool QBRFormatFB2Zip::needUnzip() {
    return true;
}
