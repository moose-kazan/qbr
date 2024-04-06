#include "qbrformatfb2.h"
#include "../qbrtemplate.h"

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

QBRFormatFB2::QBRFormatFB2() {
    parserXml = new QDomDocument();
}

QStringList QBRFormatFB2::getExtensions() { return QStringList("fb2"); }

QString QBRFormatFB2::parseXmlTextFromNode(QDomNode xmlNode) {
  if (xmlNode.isNull()) {
    return "";
  }

  if (xmlNode.isText()) {
    return xmlNode.nodeValue();
  }

  QString rv = QString("");
  for (int i = 0; xmlNode.hasChildNodes() && i < xmlNode.childNodes().count();
       i++) {
    rv.append(parseXmlTextFromNode(xmlNode.childNodes().at(i)));
  }
  return rv;
}

QString QBRFormatFB2::parseXmlBody(QDomNode xmlNode,
                                   QHash<QString, QString> xmlImages) {
  QHash<QString, QString> base_tags;
  base_tags.insert("strong", "strong");
  base_tags.insert("p", "p");
  base_tags.insert("emphasis", "em");
  base_tags.insert("code", "pre");
  base_tags.insert("sub", "sub");
  base_tags.insert("sup", "sup");
  base_tags.insert("strikethrough", "s");
  base_tags.insert("cite", "blockquote");

  QHash<QString, QString> tags_to_class;
  tags_to_class.insert("title", "doc_title");
  tags_to_class.insert("subtitle", "doc_subtitle");
  tags_to_class.insert("stanza", "doc_poem");
  tags_to_class.insert("section", "doc_section");

  QString rv = QString("");
  for (int i = 0; i < xmlNode.childNodes().count(); i++) {
    QDomNode curXmlNode = xmlNode.childNodes().at(i);
    if (curXmlNode.childNodes().count() == 0) {
      if (curXmlNode.nodeName() == "#text") {
        rv.append(parseXmlTextFromNode(curXmlNode));
      } else if (curXmlNode.nodeName() == "empty-line") {
        rv.append("<br />\n<br />\n");
      } else if (curXmlNode.nodeName() == "image") {
        QString img_src_key = QString("");
        if (curXmlNode.attributes().contains("xlink:href")) {
          img_src_key =
              curXmlNode.attributes().namedItem("xlink:href").toAttr().value();
        } else if (curXmlNode.attributes().contains("l:href")) {
          img_src_key =
              curXmlNode.attributes().namedItem("l:href").toAttr().value();
        } else if (curXmlNode.attributes().contains("href")) {
          img_src_key =
              curXmlNode.attributes().namedItem("href").toAttr().value();
        }

        if (xmlImages.value(img_src_key, "") != "") {
          rv.append("<img src=\"" + xmlImages.value(img_src_key, "") + "\" />");
        }
      }
    } else {
      if (base_tags.value(curXmlNode.nodeName(), "") != "") {
        rv.append("<" + base_tags.value(curXmlNode.nodeName()) + ">");
        rv.append(parseXmlBody(curXmlNode, xmlImages));
        rv.append("</" + base_tags.value(curXmlNode.nodeName()) + ">\n");
      } else if (tags_to_class.value(curXmlNode.nodeName(), "") != "") {
        if (curXmlNode.attributes().contains("id")) {
          rv.append("<div class=\"" +
                    tags_to_class.value(curXmlNode.nodeName()) + "\" id=\"");
          rv.append(curXmlNode.attributes().namedItem("id").toAttr().value());
          rv.append("\">");
        } else {
          rv.append("<div class=\"" +
                    tags_to_class.value(curXmlNode.nodeName()) + "\">");
        }
        rv.append(parseXmlBody(curXmlNode, xmlImages));
        rv.append("</div>\n");
      } else if (curXmlNode.nodeName() == "v") {
        rv.append(parseXmlBody(curXmlNode, xmlImages) + "<br />\n");
      } else if (curXmlNode.nodeName() == "a") {
        QString a_type = "";
        if (curXmlNode.attributes().contains("type")) {
          a_type = curXmlNode.attributes().namedItem("type").toAttr().value();
        }
        QString a_href = "";
        if (curXmlNode.attributes().contains("l:href")) {
          a_href = curXmlNode.attributes().namedItem("l:href").toAttr().value();
        } else if (curXmlNode.attributes().contains("xlink:href")) {
          a_href =
              curXmlNode.attributes().namedItem("xlink:href").toAttr().value();
        } else if (curXmlNode.attributes().contains("href")) {
          a_href = curXmlNode.attributes().namedItem("href").toAttr().value();
        }

        // Link to note
        if (a_type == "note" && a_href != "") {
          rv.append("<a class=\"doc_note_link\" href=\"" + a_href + "\">");
          rv.append(parseXmlBody(curXmlNode, xmlImages));
          rv.append("</a>\n");
        }
        // Link to all other urls
        else if (a_href != "") {
          rv.append("<a href=\"" + a_href + "\">");
          rv.append(parseXmlBody(curXmlNode, xmlImages));
          rv.append("</a>\n");
        }
        // All other cases
        else {
          rv.append(parseXmlBody(curXmlNode, xmlImages));
        }
      } else {
        rv.append(parseXmlBody(curXmlNode, xmlImages));
      }
    }
  }
  return rv;
}

bool QBRFormatFB2::parseXml(QByteArray fileData) {

  QString paserXmlErrorMsg;
  int parserXmlErrorLine;
  int parserXmlErrorColumn;
  if (!parserXml->setContent(fileData, true, &paserXmlErrorMsg,
                             &parserXmlErrorLine, &parserXmlErrorColumn)) {
    // qDebug() << "Error message: " << paserXmlErrorMsg << " at line " <<
    // parserXmlErrorLine << ", column: " << parserXmlErrorColumn;
    return false;
  }

  QHash<QString, QString> fb2_images;
  QDomNodeList fb2_binaries = parserXml->elementsByTagName("binary");
  for (int i = 0; i < fb2_binaries.count(); i++) {
    QDomNode fb2_binary = fb2_binaries.at(i);
    if (fb2_binary.attributes().contains("id") &&
        fb2_binary.attributes().contains("content-type")) {
      QString img_key = QString("#");
      img_key.append(fb2_binary.attributes().namedItem("id").toAttr().value());

      QString img_val = QString("data:");
      img_val.append(
          fb2_binary.attributes().namedItem("content-type").toAttr().value());
      img_val.append(";base64,");
      img_val.append(
          parseXmlTextFromNode(fb2_binary).replace("\n", "").replace("\r", ""));

      fb2_images.insert(img_key, img_val);
    }
  }

  htmlData.append(qbrtemplate::header());
  QDomNodeList fb2_bodies = parserXml->elementsByTagName("body");
  if (fb2_bodies.count() < 1) {
    return false;
  }

  for (int i = 0; i < fb2_bodies.count(); i++) {
    QDomNode fb2_body = fb2_bodies.at(i);
    QString fb2_body_line = "";
    if (fb2_body.attributes().contains("name")) {
      fb2_body_line.append("<div class=\"document_body\" name=\"");
      fb2_body_line.append(
          fb2_body.attributes().namedItem("name").toAttr().value());
      fb2_body_line.append("\">");
      fb2_body_line.append(parseXmlBody(fb2_body, fb2_images));
      fb2_body_line.append("</div>\n");
    } else {
      fb2_body_line.append("<div class=\"document_body\">");
      fb2_body_line.append(parseXmlBody(fb2_body, fb2_images));
      fb2_body_line.append("</div>\n");
    }
    htmlData.append(fb2_body_line);
  }
  htmlData.append(qbrtemplate::footer());

  parseBookInfo(parserXml);

  return true;
}

void QBRFormatFB2::parseBookInfo(QDomDocument *parserXml)
{
    QDomElement nodeTitleInfo = parserXml->firstChildElement("FictionBook")
            .firstChildElement("description")
            .firstChildElement("title-info");
    if (nodeTitleInfo.isNull()) {
        //qDebug() << "Book description is null";
        return;
    }

    QDomElement nodeBookTitle = nodeTitleInfo.firstChildElement("book-title");
    if (!nodeBookTitle.isNull()) {
        bookInfo.Title = parseXmlTextFromNode(nodeBookTitle);
    }

    QDomNodeList nodeAuthors = nodeTitleInfo.elementsByTagName("author");
    for (int i = 0; i < nodeAuthors.count(); i++) {
        QString firstName = parseXmlTextFromNode(nodeAuthors.at(i).firstChildElement("first-name"));
        QString lastName = parseXmlTextFromNode(nodeAuthors.at(i).firstChildElement("last-name"));
        if (firstName == "" && lastName == "") {
            continue;
        }
        QString middleName = parseXmlTextFromNode(nodeAuthors.at(i).firstChildElement("middle-name"));

        QString authorName = firstName;
        if (middleName != "") {
            authorName += " " + middleName;
        }
        authorName += " " + lastName;

        if (bookInfo.Author == "") {
            bookInfo.Author = authorName;
        }
        else {
            bookInfo.Author += ", " + authorName;
        }
    }

    //qDebug() << "Book title" << bookInfo.Title;
    //qDebug() << "Book author" << bookInfo.Author;
}

bool QBRFormatFB2::loadFile(QString fileName, QByteArray fileData) {
  htmlData = "";  // reset data from previous file
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

QBRBook QBRFormatFB2::getBook()
{
    return QBRBook{bookInfo, htmlData};
}
