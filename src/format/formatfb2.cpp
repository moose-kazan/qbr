#include "formatfb2.h"
#include "../template.h"

#include <QByteArray>
#include <QDataStream>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QFile>
#include <QHash>
#include <QRegularExpression>
#include <QString>

#include <QDebug>

FormatFB2::FormatFB2() {}

QStringList FormatFB2::getExtensions() { return QStringList("fb2"); }

QDomNode FormatFB2::parseXmlNode(QDomNode currentNode, QHash<QString, QString> fb2Binaries) {
    (void) fb2Binaries;

    QHash<QString, QString> baseTags = {
        {"strong",        "strong"},
        {"p",             "p"},
        {"emphasis",      "em"},
        {"code",          "pre"},
        {"sub",           "sub"},
        {"sup",           "sup"},
        {"strikethrough", "s"},
        {"cite",          "blockquote"},
        {"empty-line",    "br"},
        {"image",         "img"},
        {"a",             "a"},
        {"table",         "table"},
        {"tr",            "tr"},
        {"th",            "th"},
        {"td",            "td"},
    };

    QHash<QString, QString> tagToClass = {
        {"body",     "document_body"},
        {"title",    "doc_title"},
        {"subtitle", "doc_subtitle"},
        {"stanza",   "doc_poem"},
        {"poem",     "doc_poem"},
        {"section",  "doc_section"},
    };

    QList<QString> allowedAttributes = {"id", "name"};

    switch (currentNode.nodeType()) {
    case QDomNode::ElementNode:
        {
            QString returnTagName = "div";
            QString currentNodeTag = currentNode.nodeName().toLower();

            if (baseTags.contains(currentNodeTag)) {
                returnTagName = baseTags.value(currentNodeTag);
            }
            /*else {
                qDebug() << "nodeName" << currentNode.nodeName() << "nodeValue" << currentNode.nodeValue();
            }*/

            QDomElement returnValue = QDomDocument().createElement(returnTagName);

            if (tagToClass.contains(currentNodeTag)) {
                returnValue.setAttribute("class", tagToClass.value(currentNodeTag));
            }

            if (returnTagName.compare("img") == 0) {
                if (currentNode.attributes().contains("href")) {
                    QString imgHref = currentNode.attributes().namedItem("href").nodeValue();
                    returnValue.setAttribute("src", fb2Binaries.value(imgHref));
                }
            }
            else if (returnTagName.compare("a") == 0) {
                if (currentNode.attributes().contains("href")) {
                    QString aHref = currentNode.attributes().namedItem("href").nodeValue();
                    //qDebug() << "a.href" << aHref;
                    returnValue.setAttribute("href", aHref);
                    returnValue.setAttribute("title", aHref);
                }
                if (currentNode.attributes().contains("type")) {
                    QString aType = currentNode.attributes().namedItem("type").nodeValue();
                    if (aType.compare("note") == 0) {
                        returnValue.setAttribute("class", "doc_note_link");
                    }
                }
            }

            for (int i = 0; (currentNode.hasAttributes() && i < allowedAttributes.count()); i++) {
                QString attrName = allowedAttributes.at(i);
                if (currentNode.attributes().contains(attrName)) {
                    returnValue.setAttribute(attrName, currentNode.attributes().namedItem(attrName).nodeValue());
                }
            }

            if (currentNode.hasChildNodes()) {
                for (int i = 0; i < currentNode.childNodes().count(); i++) {
                    QDomNode localNode = currentNode.childNodes().at(i);

                    returnValue.appendChild(parseXmlNode(localNode, fb2Binaries));
                }
            }
            // DIrty hack. But without it we have broken xHTML in some cases
            else {
                returnValue.appendChild(QDomDocument().createTextNode(""));
            }

            return returnValue;
        }
        break;
    case QDomNode::TextNode:
        return currentNode.cloneNode();
        break;

    case QDomNode::EntityReferenceNode:
        return currentNode.cloneNode();
        break;
    default:
        //qDebug() << "nodeType" << currentNode.nodeType() << "nodeName" << currentNode.nodeName() << "nodeValue" << currentNode.nodeValue() << "count" << currentNode.childNodes().count();
        break;
    }

    return QDomDocument().createTextNode("");
}

bool FormatFB2::parseXml(QByteArray fileData) {

  QString paserXmlErrorMsg;
  int parserXmlErrorLine;
  int parserXmlErrorColumn;
  QDomDocument parserXml;
  if (!parserXml.setContent(fileData, true, &paserXmlErrorMsg,
                             &parserXmlErrorLine, &parserXmlErrorColumn)) {
    // qDebug() << "Error message: " << paserXmlErrorMsg << " at line " <<
    // parserXmlErrorLine << ", column: " << parserXmlErrorColumn;
    return false;
  }

  QHash<QString, QString> fb2Binaries;
  QDomElement fb2Binary = parserXml.firstChildElement("FictionBook").firstChildElement("binary");
  while (!fb2Binary.isNull()) {
      QString binaryId = QString("#").append(fb2Binary.attribute("id", ""));
      QString binaryContentType = fb2Binary.attribute("content-type", "");
      QString binaryValue = QString("data:%1;base64,%2").arg(binaryContentType).arg(fb2Binary.text());

      fb2Binaries.insert(binaryId, binaryValue);

      fb2Binary = fb2Binary.nextSiblingElement("binary");
  }


  htmlData.append(Template::header());
  QDomElement fb2Body = parserXml.firstChildElement("FictionBook").firstChildElement("body");

  if (fb2Body.isNull()) {
      return false;
  }

  while (!fb2Body.isNull()) {
      QDomNode convertedNode = parseXmlNode(fb2Body, fb2Binaries);
      QString nodeAsString;
      QTextStream nodeStream(&nodeAsString);
      convertedNode.save(nodeStream, 2);
      htmlData.append(nodeAsString);

      fb2Body = fb2Body.nextSiblingElement("body");
  }

  htmlData.append(Template::footer());

  parseBookInfo(&parserXml);

  return true;
}

void FormatFB2::parseBookInfo(QDomDocument *parserXml) {
  QDomElement nodeTitleInfo = parserXml->firstChildElement("FictionBook")
                                  .firstChildElement("description")
                                  .firstChildElement("title-info");
  if (nodeTitleInfo.isNull()) {
    // qDebug() << "Book description is null";
    return;
  }

  QDomElement nodeBookTitle = nodeTitleInfo.firstChildElement("book-title");
  if (!nodeBookTitle.isNull()) {
      bookInfo.Title = nodeBookTitle.text();
  }

  QDomNodeList nodeAuthors = nodeTitleInfo.elementsByTagName("author");
  for (int i = 0; i < nodeAuthors.count(); i++) {
    QString firstName = nodeAuthors.at(i).firstChildElement("first-name").text();
    QString lastName = nodeAuthors.at(i).firstChildElement("last-name").text();
    if (firstName == "" && lastName == "") {
      continue;
    }
    QString middleName = nodeAuthors.at(i).firstChildElement("middle-name").text();

    QString authorName = firstName;
    if (middleName != "") {
      authorName += " " + middleName;
    }
    authorName += " " + lastName;

    if (bookInfo.Author == "") {
      bookInfo.Author = authorName;
    } else {
      bookInfo.Author += ", " + authorName;
    }
  }


  // qDebug() << "Book title" << bookInfo.Title;
  // qDebug() << "Book author" << bookInfo.Author;
}

bool FormatFB2::loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) {
  (void)zipData;
  htmlData = ""; // reset data from previous file
  bookInfo = {};
  bookInfo.FileFormat = "FictionBook 2";

  (void)fileName; // Remove "unused parameter" warning
  try {
    return parseXml(fileData);
  } catch (...) {
    return false;
  }

  return false;
}

QBRBook FormatFB2::getBook() { return QBRBook{bookInfo, htmlData}; }

bool FormatFB2::needUnzip() { return false; }
