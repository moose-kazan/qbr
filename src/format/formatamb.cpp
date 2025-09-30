#include "formatamb.h"

#include <QBuffer>
#include <QCollator>
#include <QString>
#include <QUrl>
#include <QtEndian>

/*
 * AMB support
 *
 * Based on:
 * https://ambook.sourceforge.net/phpamb.php?fname=archiwum/format-20201216.amb
 *
 * Some ideas found here:
 * https://sourceforge.net/p/utf8tocp/code/HEAD/tree/trunk/utf8tocp.c And here:
 * https://osdn.net/projects/amb/scm/svn/tree/head/phpamb/
 *
 * mvucomp-functions based on original code and created with help of GigaCode AI.
 */

FormatAMB::FormatAMB()
{
  for (int i = 0; i < 128; i++)
    unicodeMap[i] = defaultUnicodeMap[i];
}

QStringList FormatAMB::getExtensions() { return QStringList("amb"); }

QString FormatAMB::getFormatTitle()
{
  return "Ancient Machine's Book";
}

bool FormatAMB::parseAmb(const QByteArray& fileData) {
  // Check file signature
  if (!fileData.startsWith("AMB1")) {
    return false;
  }

  const int filesCount = qFromLittleEndian<qint16>(fileData.mid(4, 2).data());

  for (int i = 0; i < filesCount; i++) {
    QString entryName = "";
    const int entryOffset = 6 + i * 20;
    for (int j = 0; j < 12 && fileData.at(entryOffset + j) != 0; j++) {
      entryName.append(fileData.at(entryOffset + j));
    }
    const int entryFileOffset =
        qFromLittleEndian<quint32>(fileData.mid(entryOffset + 12, 4).data());
    const int entryFileLength =
        qFromLittleEndian<quint16>(fileData.mid(entryOffset + 16, 2).data());
    if (entryName.endsWith(".amc", Qt::CaseInsensitive))
    {
      ambEntries.insert(entryName.toLower(),
                        mvucomp(fileData.mid(entryFileOffset, entryFileLength)));

    }
    else
    {
      ambEntries.insert(entryName.toLower(),
                        fileData.mid(entryFileOffset, entryFileLength));
    }

    // qDebug() << entryName << " " << entryFileOffset << " " <<
    // entryFileLength;
  }

  // If file have character map
  if (ambEntries.contains("unicode.map")) {
    for (int i = 0; i < 128; i++) {
      unicodeMap[i] = qFromLittleEndian<qint16>(
          ambEntries.value("unicode.map").mid(i * 2, 2).data());
    }
  }

  // If file have title
  if (ambEntries.contains("title")) {
    bookInfo.Title = convertToUtf8(ambEntries.value("title"))
                         .replace("\n", "")
                         .replace("\r", "");
  }

  templateInit();

  if (ambEntries.contains("index.ama"))
  {
    //htmlBodyData.append(amaToHtml("index.ama"));
    QString amaData = amaToHtml("index.ama");
    QDomDocument docPart = QDomDocument();
    docPart.setContent(amaData);
    templateBodyAppend(docPart);
  }
  else if (ambEntries.contains("index.amc"))
  {
    //htmlBodyData.append(amaToHtml("index.amc"));
    QString amaData = amaToHtml("index.amc");
    QDomDocument docPart = QDomDocument();
    docPart.setContent(amaData);
    templateBodyAppend(docPart);
  }
  else
  {
    return false;
  }

  // Sort entry names with QCollator. It's fix some issues with numbers
  QStringList entryNames = ambEntries.keys();
  QCollator collator;
  collator.setNumericMode(true);

  for (int i = 0; i < entryNames.length() - 1; i++) {
    for (int j = i + 1; j < entryNames.length(); j++) {
      if (const int r = collator.compare(entryNames.at(i), entryNames.at(j)); r > 0) {
        QString tmp = entryNames.at(i);
        entryNames.replace(i, entryNames.at(j));
        entryNames.replace(j, tmp);
      }
    }
  }

  for (int i = 0; i < entryNames.length(); i++) {
    if (
        entryNames.at(i) != "index.ama" &&
        entryNames.at(i) != "index.amc" &&
        (entryNames.at(i).endsWith(".ama") ||  entryNames.at(i).endsWith(".amc"))) {
      //htmlBodyData.append(amaToHtml(entryNames.at(i)));
      QDomDocument docPart;
      QString amaData = amaToHtml(entryNames.at(i));
      docPart.setContent(amaData);
      templateBodyAppend(docPart);
    }
  }

  templateSetMeta(bookInfo);
  htmlData = templateAsString();
  //htmlData.insert(htmlData.indexOf("</body>"), htmlBodyData);

  // htmlData = "Total files: " + QString::number(filesCount) + "!\n";

  return true;
}

QString FormatAMB::convertToUtf8(const QByteArray& fileData) const
{
  QString rv;
  for (const unsigned char cur_char : fileData) {
    if (cur_char < 128) {
      rv.append(QChar(cur_char));
    } else {
      unsigned short new_char = 0;
      new_char = unicodeMap[cur_char - 128];
      rv.append(QChar(new_char));
    }
  }
  return rv;
}

QString FormatAMB::amaToHtml(const QString& fileName) const
{
  QString rv;

  QString sectionId = fileName;
  if (sectionId.endsWith(".ama") || sectionId.endsWith(".amc"))
  {
    sectionId.chop(4);
  }
  rv.append(R"(<div class="amb_body" id=")");
  rv.append(sectionId);
  rv.append("\">\n");

  rv.append(R"(<div class="amb_part_header">)");
  rv.append("Part name: ");
  rv.append(fileName);
  rv.append(R"(. <a href="#index">Go to index</a>.)");
  rv.append("</div>\n");

  const QString fileData = convertToUtf8(ambEntries.value(fileName, ""));

  bool readlink = false;
  bool escNow = false;
  QString openTag = "";

  for (int i = 0; i < fileData.length(); i++) {
    QChar c = fileData.at(i);
    if (c == QChar(0x0d)) {
      continue;
    }

    if (readlink) {
      if (c == QChar(0x0a) || c == ':') {
        if (rv.endsWith(".ama") || rv.endsWith(".amc"))
        {
          rv.chop(4);
        }
        rv.append("\">");
        openTag = "a";
        readlink = false;

      } else {
        rv.append(QUrl::toPercentEncoding(c.toLower()));
      }
      continue;
    }

    if (escNow) {
      if (c == '%') {
        rv.append("%");
      } else if (c == 'l') {
        rv.append("<a href=\"#");
        readlink = true;
      } else if (c == 'h') {
        rv.append("<h1>");
        openTag = "h1";
      } else if (c == '!') {
        rv.append("<span class=\"amb_notice\">");
        openTag = "span";
      } else if (c == 'b') {
        rv.append("<span class=\"amb_boring\">");
        openTag = "span";
      }

      escNow = false;
      continue;
    }

    if (openTag.length() > 0 && (c == QChar(0x0a) || c == '%')) {
      // if file end with spaces
      for (int j = rv.length() - 1; j >= 0; j--) {
        if (rv.at(j) != ' ') {
          rv.insert(j + 1, "</" + openTag + ">");
          break;
        }
      }
      openTag = "";
    }

    if (c == '%') {
      escNow = true;
    } else if (c == QChar(0x0a)) {
      rv.append("<br />\n");
    } else {
      rv.append(QString(c).toHtmlEscaped());
      // rv.append(c);
    }
  }

  if (openTag.length() > 0) {
    rv.append("</");
    rv.append(openTag);
    rv.append(">");
  }

  rv.append("</div>\n");
  rv.append("<hr />\n");

  return rv;
}

bool FormatAMB::loadFile(const QString fileName, const QByteArray fileData, qbrunzip *zipData) {
  (void)zipData;
  // reset data from previous file
  htmlData = "";
  bookInfo.clear();
  bookInfo.FileFormat = getFormatTitle();

  ambEntries.clear();
  for (int i = 0; i < 128; i++)
    unicodeMap[i] = defaultUnicodeMap[i];

  /*
   * AMB file have hard-defined signature. We use it to detect
   *
  QRegExp rx("\\.amb$", Qt::CaseInsensitive);
  if (rx.indexIn(fileName) < 0)
  {
      return false;
  }
  */
  (void)fileName; // Remove "unused parameter" warning
  try {
    return parseAmb(fileData);
  } catch (...) {
    //return false;
  }
  return false;
}

QBRBook FormatAMB::getBook() { return QBRBook{bookInfo, htmlData}; }

bool FormatAMB::needUnzip() { return false; }

unsigned short FormatAMB::mvcomp_depack(unsigned char *dst, unsigned short buff16) {
  static unsigned char rawwords; // количество несжатых слов
  unsigned char *dst_start = dst;

  if (rawwords != 0) {
    unsigned short *dst16 = reinterpret_cast<unsigned short*>(dst);
    *dst16 = buff16;
    dst += 2;
    rawwords--;
  } else if ((buff16 & 0xF000) == 0) {
    *dst = buff16 & 0xFF;
    dst++;
    rawwords = buff16 >> 8;
  } else {
    unsigned char *src = dst - (buff16 & 0x0FFF) - 1;
    buff16 >>= 12;
    for (;;) {
      *dst = *src;
      dst++; src++;
      if (buff16 == 0) break;
      buff16--;
    }
  }

  return static_cast<unsigned short>(dst - dst_start);
}


QByteArray FormatAMB::mvucomp(const QByteArray& inputData)
{
  if (inputData.size() < 2)
  {
    return {};
  }

  quint16 mvSignature;
  std::memcpy(&mvSignature, inputData.constData(), sizeof(mvSignature));
  if ((mvSignature & 0xF000) != 0) {
    return {};
  }

  QVector<uchar> buffer(8192); // Буфер размером 8 КБ
  uchar* buffptr = buffer.data();

  unsigned long bytesIn = 0;
  unsigned long bytesOut = 0;

  QByteArray outData;
  QBuffer outBuffer(&outData);
  if (!outBuffer.open(QIODevice::WriteOnly))
  {
    return {};
  }

  for (int i = 0; i < inputData.size(); i += 2)
  {
    if (i + 1 >= inputData.size()) break;

    quint16 token;
    std::memcpy(&token, inputData.constData() + i, sizeof(token));
    bytesIn += 2;

    unsigned short len = mvcomp_depack(buffptr, token);
    bytesOut += len;

    outBuffer.write(reinterpret_cast<char*>(buffptr), len);
    buffptr += len;

    if (buffptr - buffer.data() > 6144) {
      std::memmove(buffer.data(), buffptr - 2048, 6144);
      buffptr = buffer.data() + 6144;
    }

  }

  outBuffer.close();
  return outData;
};