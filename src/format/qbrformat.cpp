#include "qbrformat.h"

QBRFormat::QBRFormat() {}

bool QBRFormat::loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) {
    (void)fileName;
    (void)fileData;
    (void)zipData;
    return false;
};

QBRBook QBRFormat::getBook() {
    return QBRBook{};
};

QStringList QBRFormat::getExtensions() {
    return QStringList();
};

bool QBRFormat::needUnzip() {
    return false;
};


bool QBRFormat::isZipFile(QByteArray data) {
    char zipSignature[] = {0x50, 0x4B, 0x03, 0x04};
    return data.startsWith(zipSignature);
}
