#include "bookloader.h"

#include <QFile>

BookLoader::BookLoader() {
    bookData = new QBRBook();
    QString filterLine;
    QStringList allExtensions;

    unZip = new qbrunzip(false);
    for (int i = 0; i < bookParsers.count(); i++) {
        QStringList formatExtensions = bookParsers.at(i)->getExtensions();

        formatExtensions.replaceInStrings(re, "*.");
        filterLine = QString("%1 (%2)").arg(bookParsers.at(i)->getFormatTitle(), formatExtensions.join(" "));
        filtersData.append(filterLine);

        allExtensions.append(formatExtensions);
    }

    filterLine = QString(tr("Books (%1)")).arg(allExtensions.join(" "));
    filtersData.append(filterLine);
}

QStringList BookLoader::getFilter()
{
    return filtersData;
}


QBRBook* BookLoader::getBook() const
{
    return bookData;
}

bool BookLoader::loadFile(const QString& fileName)
{
    unZip->clear();

    QByteArray fileData;
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly)) {
        return false;
    }
    QDataStream in(&f);
    constexpr int buff_size = 4096;
    char buff[buff_size];
    while (!in.atEnd()) {
        if (const long readed = in.readRawData(buff, buff_size); readed > 0) {
            fileData.append(buff, readed);
        }
    }
    f.close();

    const bool isZipFile = Format::isZipFile(fileData);

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
