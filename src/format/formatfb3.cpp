#include "formatfb3.h"
#include "../template.h"

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QUrl>

/*
 * Format description:
 * http://www.fictionbook.org/index.php/FictionBook_3.0_beta_-_%D0%BA%D1%80%D0%B0%D1%82%D0%BA%D0%BE%D0%B5_%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5
 * Many ideas found at: https://github.com/gribuser/FB3
 */

FormatFB3::FormatFB3() {}

QStringList FormatFB3::getExtensions() { return QStringList("fb3"); }

bool FormatFB3::loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) {
  // reset data from previous file
  htmlData = "";
  bookInfo.clear();
  bookInfo.FileFormat = "FictionBook 3";

  fb3_binaries.clear();

  QRegularExpression rx("\\.fb3$", QRegularExpression::CaseInsensitiveOption);
  if (!rx.match(fileName).hasMatch()) {
    return false;
  }

  if (!isZipFile(fileData)) {
    return false;
  }

  unZip = zipData;

  if (!unZip->isLoaded()) {
      return false;
  }

  return parseFile();
}

QString FormatFB3::parseFB3TextFromNode(QDomNode xmlNode) {
  if (xmlNode.isNull()) {
    return "";
  }

  if (xmlNode.isText()) {
    return xmlNode.nodeValue();
  }

  QString rv = QString("");
  for (int i = 0; xmlNode.hasChildNodes() && i < xmlNode.childNodes().count();
       i++) {
    rv.append(parseFB3TextFromNode(xmlNode.childNodes().at(i)));
  }
  return rv;
}

QString FormatFB3::parseFB3Node(QDomNode xmlNode) {
  QHash<QString, QString> base_tags;
  base_tags.insert("strong", "strong");
  base_tags.insert("p", "p");
  base_tags.insert("em", "em");
  base_tags.insert("code", "pre");
  base_tags.insert("sub", "sub");
  base_tags.insert("sup", "sup");
  base_tags.insert("strikethrough", "s");
  base_tags.insert("u", "u");
  base_tags.insert("b", "b");
  base_tags.insert("i", "i");
  base_tags.insert("blockquote", "blockquote");
  base_tags.insert("div", "div");

  QHash<QString, QString> tag_to_class;
  tag_to_class.insert("title", "doc_title");
  tag_to_class.insert("subtitle", "doc_subtitle");
  tag_to_class.insert("poem", "doc_poem");
  tag_to_class.insert("stanza", "doc_poem");
  tag_to_class.insert("section", "doc_section");
  tag_to_class.insert("notebody", "doc_note_body");
  tag_to_class.insert("notes", "doc_notes");
  tag_to_class.insert("epigraph", "doc_epigraph");
  tag_to_class.insert("annotation", "doc_annotation");

  QList<QString> bad_urls_schemes;
  bad_urls_schemes.append("data");
  bad_urls_schemes.append("javascript");

  QString rv;
  if (xmlNode.hasChildNodes()) {
    for (int i = 0; i < xmlNode.childNodes().count(); i++) {
      QDomNode curXmlNode = xmlNode.childNodes().at(i);
      if (tag_to_class.value(curXmlNode.nodeName(), "") != "") {
        if (curXmlNode.attributes().contains("id")) {
          QString section_id =
              curXmlNode.attributes().namedItem("id").toAttr().value();
          rv.append("<div class=\"");
          rv.append(tag_to_class.value(curXmlNode.nodeName()));
          rv.append("\" id=\"" + section_id + "\">\n");
        } else {
          rv.append("<div class=\"");
          rv.append(tag_to_class.value(curXmlNode.nodeName()));
          rv.append("\">\n");
        }
        rv.append(parseFB3Node(curXmlNode));
        rv.append("</div>\n");
      } else if (base_tags.value(curXmlNode.nodeName(), "") != "") {
        rv.append("<" + base_tags.value(curXmlNode.nodeName()) + ">");
        rv.append(parseFB3Node(curXmlNode));
        rv.append("</" + base_tags.value(curXmlNode.nodeName()) + ">");
      } else if (curXmlNode.nodeName() == "img") {
        QString img_src_key =
            curXmlNode.attributes().namedItem("src").toAttr().value();
        if (fb3_binaries.contains(img_src_key)) {
          rv.append("<img src=\"");
          rv.append(fb3_binaries.value(img_src_key));
          rv.append("\">\n");
        } else {
          rv.append("<p>[ImgNotFound! Id=" + img_src_key + "]</p>\n");
        }
      } else if (curXmlNode.nodeName() == "a") {
        QString a_href = "";
        if (curXmlNode.attributes().contains("l:href")) {
          a_href = curXmlNode.attributes().namedItem("l:href").toAttr().value();
        } else if (curXmlNode.attributes().contains("xlink:href")) {
          a_href =
              curXmlNode.attributes().namedItem("xlink:href").toAttr().value();
        } else if (curXmlNode.attributes().contains("href")) {
          a_href = curXmlNode.attributes().namedItem("href").toAttr().value();
        }

        if (a_href != "") {
          rv.append("<a href=\"" + a_href + "\">");
          rv.append(parseFB3Node(curXmlNode));
          rv.append("</a>");
        } else {
          rv.append(parseFB3Node(curXmlNode));
        }

      } else {
        // qDebug() << curXmlNode.nodeName();
        rv.append(parseFB3Node(curXmlNode));
      }
    }
  } else {
    // qDebug() << xmlNode.nodeName();
    if (xmlNode.nodeName() == "xml") {
      // Nothing to do
    } else if (xmlNode.nodeName() == "br") {
      rv.append("<br />\n");
    }
    rv.append(parseFB3TextFromNode(xmlNode));
  }
  return rv;
}

void FormatFB3::parseFB3Metadata(QString entryName) {
    QByteArray fb3_description_data(unZip->getFileData(entryName));
  if (fb3_description_data.isNull()) {
    return;
  }

  QDomDocument descriptionXml;
  if (!descriptionXml.setContent(fb3_description_data)) {
    return;
  }

  QDomElement nodeBookTitle =
      descriptionXml.firstChildElement("fb3-description")
          .firstChildElement("title")
          .firstChildElement("main");
  if (!nodeBookTitle.isNull()) {
    bookInfo.Title = parseFB3TextFromNode(nodeBookTitle);
  }

  QDomNodeList nodeAuthors = descriptionXml.firstChildElement("fb3-description")
                                 .elementsByTagName("subject");

  for (int i = 0; i < nodeAuthors.length(); i++) {
    QDomNode nodeAuthor = nodeAuthors.at(i);
    if (!nodeAuthor.attributes().contains("link")) {
      continue;
    }
    if (nodeAuthor.attributes().namedItem("link").toAttr().value() !=
        "author") {
      continue;
    }
    QString firstName =
        parseFB3TextFromNode(nodeAuthor.firstChildElement("first-name"));
    QString lastName =
        parseFB3TextFromNode(nodeAuthor.firstChildElement("last-name"));
    if (firstName == "" && lastName == "") {
      continue;
    }
    QString middleName =
        parseFB3TextFromNode(nodeAuthor.firstChildElement("middle-name"));

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
}

bool FormatFB3::parseFile() {
    QStringList zipEntryNames = unZip->getFileNameList();
    QStringList requiredFiles = {
        "[Content_Types].xml",
        "_rels/.rels",
    };

    for (int i = 0; i < requiredFiles.count(); i++) {
        if (!zipEntryNames.contains(requiredFiles.at(i), Qt::CaseInsensitive)) {
            // qDebug() << "can't find file: " << requiredFiles.at(i);
            return false;
        }
    }

    QByteArray docContentTypeData = unZip->getFileData(zipEntryNames.filter("[Content_Types].xml", Qt::CaseInsensitive).at(0));
    if (docContentTypeData.isNull()) {
        // qDebug() << "can't extract [Content_Types].xml";
        return false;
    }

  QDomDocument docContentType;
  QString docContentTypeErrorMsg;
  if (!docContentType.setContent(docContentTypeData, true,
                                 &docContentTypeErrorMsg)) {
    qDebug() << "can't parse [Content_Types].xml" << docContentTypeErrorMsg;
    return false;
  }

  // Default file name of body entry
  QString body_entry_name = "fb3/body.xml";

  // Default file name of description entry
  QString description_entry_name = "fb3/description.xml";

  // Extensions -> Content-TYpe
  QHash<QString, QString> fb3_ext_types;

  // Overrides
  QHash<QString, QDomNode> fb3_overrides;

  // Load content-types for extensions
  QDomNodeList fb3_default_nodes = docContentType.elementsByTagName("Default");
  for (int i = 0; i < fb3_default_nodes.count(); i++) {
    QDomNode default_node = fb3_default_nodes.at(i);
    if (default_node.attributes().contains("Extension") &&
        default_node.attributes().contains("ContentType")) {
      fb3_ext_types.insert(
          default_node.attributes().namedItem("Extension").toAttr().value(),
          default_node.attributes().namedItem("ContentType").toAttr().value());
    }
  }

  // Try to find bodies and load overrides
  QDomNodeList fb3_overrides_nodes =
      docContentType.elementsByTagName("Override");
  for (int i = 0; i < fb3_overrides_nodes.count(); i++) {
    QDomNode override_node = fb3_overrides_nodes.at(i);
    if (override_node.attributes().contains("PartName") &&
        override_node.attributes().contains("ContentType")) {
      QString overridePartName =
          override_node.attributes().namedItem("PartName").toAttr().value();
      if (overridePartName.startsWith("/")) {
        overridePartName =
            overridePartName.right(overridePartName.length() - 1);
      } else if (overridePartName.startsWith("./")) {
        overridePartName =
            overridePartName.right(overridePartName.length() - 2);
      }

      if (override_node.attributes()
              .namedItem("ContentType")
              .toAttr()
              .value() == "application/fb3-body+xml") {
        body_entry_name = overridePartName;
      } else if (override_node.attributes()
                     .namedItem("ContentType")
                     .toAttr()
                     .value() == "application/fb3-description+xml") {
        description_entry_name = overridePartName;
      } else {
        fb3_overrides.insert(overridePartName, override_node);
      }
    }
  }

  // Process metadata
  parseFB3Metadata(description_entry_name);

  // Try to load body
  QByteArray fb3_body_data(unZip->getFileData(body_entry_name));
  if (fb3_body_data.isNull()) {
    // qDebug() << "can't extract fb3 body";
    return false;
  }

  QDomDocument bodyXml;
  if (!bodyXml.setContent(fb3_body_data)) {
    // qDebug() << "can't parse fb3 body";
    return false;
  }

  // Body rels file name
  QString body_rels_entry_name;
  QString body_entry_base_path;
  QFileInfo body_entry_name_info(body_entry_name);

  body_entry_base_path = body_entry_name_info.path();

  if (body_entry_base_path == '.') {
    body_entry_base_path = "";
  } else if (!body_entry_base_path.endsWith('/')) {
    body_entry_base_path.append('/');
  }
  body_rels_entry_name = body_entry_base_path;
  body_rels_entry_name.append("_rels/");
  body_rels_entry_name.append(body_entry_name_info.fileName());
  body_rels_entry_name.append(".rels");

  // Try to load body rels file
  QByteArray body_rels_data(unZip->getFileData(body_rels_entry_name));
  if (body_rels_data.isNull()) {
    return false;
  }

  QDomDocument bodyRelsXml;
  if (!bodyRelsXml.setContent(body_rels_data)) {
    return false;
  }

  // Load binaries
  QDomNodeList relation_nodes = bodyRelsXml.elementsByTagName("Relationship");
  for (int i = 0; i < relation_nodes.count(); i++) {
    QDomNode relation_node = relation_nodes.at(i);
    if (relation_node.attributes().contains("Id") &&
        relation_node.attributes().contains("Target")) {
      QString node_id =
          relation_node.attributes().namedItem("Id").toAttr().value();
      QString node_target =
          relation_node.attributes().namedItem("Target").toAttr().value();
      if (node_target.startsWith("/")) {
        node_target = node_target.right(node_target.length() - 1);
      } else if (node_target.startsWith("./")) {
        node_target = node_target.right(node_target.length() - 2);
        node_target.prepend(body_entry_base_path);
      } else {
        node_target.prepend(body_entry_base_path);
      }

      QString node_content_type;

      if (fb3_ext_types.contains(node_target.section('.', -1))) {
        node_content_type = fb3_ext_types.value(node_target.section('.', -1));
      }

      // If we have override for this file
      if (fb3_overrides.contains(node_target)) {
        if (fb3_overrides.value(node_target)
                .attributes()
                .contains("ContentType")) {
          node_content_type = fb3_overrides.value(node_target)
                                  .attributes()
                                  .namedItem("ContentType")
                                  .toAttr()
                                  .value();
        }
      }

      QByteArray node_entry_data(unZip->getFileData(node_target));
      if (node_entry_data.isNull()) {
        return false;
      }

      fb3_binaries.insert(node_id, "data:" + node_content_type + ";base64," +
                                       node_entry_data.toBase64());

      // qDebug() << "Id: " << node_id << " Target: " << node_target << " Type:
      // " << node_content_type;
    }
  }

  // Process body
  htmlData.append(Template::header());
  htmlData.append("<div class=\"document_body\">\n");
  for (int i = 0; i < bodyXml.childNodes().count(); i++) {

    htmlData.append(parseFB3Node(bodyXml.childNodes().at(i)));
  }
  htmlData.append("</div>\n");
  htmlData.append(Template::footer());

  return true;
}

QBRBook FormatFB3::getBook() { return QBRBook{bookInfo, htmlData}; }

bool FormatFB3::needUnzip() { return true; }
