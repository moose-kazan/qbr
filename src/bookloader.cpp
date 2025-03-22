#include "bookloader.h"

#include <QFile>

BookLoader::BookLoader() {
    unZip = new qbrzip(false);
}

QStringList BookLoader::getExtensions()
{
    QStringList allExt;
    for (int i = 0; i < bookParsers.count(); i++) {
        allExt.append(bookParsers.at(i)->getExtensions());
    }
    return allExt;
};

QBRBook BookLoader::getBook()
{
    return bookData;
}

bool BookLoader::loadFile(QString fileName)
{
    unZip->clear();

    QByteArray fileData;
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QDataStream in(&f);
    int buff_size = 4096;
    char buff[buff_size];
    while (!in.atEnd()) {
        int readed = in.readRawData(buff, buff_size);
        if (readed > 0) {
            fileData.append(buff, readed);
        }
    }
    f.close();

    bool isZipFile = Format::isZipFile(fileData);

    for (int i = 0; i < bookParsers.count(); i++) {
        if (bookParsers.at(i)->needUnzip()) {
            if (!isZipFile) {
                continue;
            }

            if (!unZip->isLoaded()) {
                unZip->setData(fileData);
            }
        }

        if (bookParsers.at(i)->loadFile(fileName, fileData, unZip)) {
            bookData = bookParsers.at(i)->getBook();
            return true;
        }
    }

    return false;
}
