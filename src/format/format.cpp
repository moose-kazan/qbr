#include "format.h"

Format::Format() {}

bool Format::loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) {
    (void)fileName;
    (void)fileData;
    (void)zipData;
    return false;
};

QBRBook Format::getBook() {
    return QBRBook{};
};

QStringList Format::getExtensions() {
    return QStringList();
};

QString Format::getFormatTitle()
{
    return tr("Unknown file format");
}


bool Format::needUnzip() {
    return false;
};


bool Format::isZipFile(const QByteArray& data) {
    char zipSignature[] = {0x50, 0x4B, 0x03, 0x04};
    return data.startsWith(zipSignature);
}
