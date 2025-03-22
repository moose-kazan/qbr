#include "qbrzip.h"

#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QStringList>

#include <zip.h>

// Based on https://gist.github.com/mobius/1759816
// Some ideas found here: https://github.com/ctabin/libzippp/blob/master/src/

qbrzip::qbrzip(bool CS) {
    entry_names_cs = CS;
    loaded = false;
}

bool qbrzip::setData(QByteArray zipData) {
  zip_error_t zipError;
  int buff_size = 4096;
  char buff[buff_size];
  loaded = false;

  zip_source_t *src =
      zip_source_buffer_create(zipData.data(), zipData.size(), 1, &zipError);

  if (src == NULL) {
    return false;
  }

  zip *zf = zip_open_from_source(src, 0, &zipError);

  if (zf == NULL) {
    return false;
  }

  zip_int64_t filesCount = zip_get_num_entries(zf, 0);

  for (int i = 0; i < filesCount; i++) {
    QString zipEntryName = zip_get_name(zf, i, 0);

    // qDebug() << "Filename: " << zipEntryName;

    if (zipEntryName.at(zipEntryName.length() - 1) == '/') {
      continue;
    }

    QByteArray zipEntryData;

    zip_file *f = zip_fopen_index(zf, i, 0);
    while (true) {
      zip_int64_t bytesRead = zip_fread(f, &buff, buff_size);
      if (bytesRead == 0) {
        break;
      } else if (bytesRead > 0) {
        zipEntryData.append(buff, bytesRead);
      } else if (bytesRead < 0) {
        return false;
      }
    }
    zip_fclose(f);

    if (!entry_names_cs) {
      zipEntryName = zipEntryName.toLower();
    }

    zipEntries.insert(zipEntryName, zipEntryData);
  }

  loaded = true;
  return true;
}

QStringList qbrzip::getFileNameList() {
  QStringList rv(zipEntries.keys());
  return rv;
}

QByteArray qbrzip::getFileData(QString fileName) {
  if (!entry_names_cs) {
    fileName = fileName.toLower();
  }
  return zipEntries.value(fileName, NULL);
}

void qbrzip::clear() {
    zipEntries.clear();
    loaded = false;
}

bool qbrzip::isLoaded()
{
    return loaded;
}
