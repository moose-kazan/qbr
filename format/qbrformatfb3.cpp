#include "qbrformatfb3.h"
#include "../qbrtemplate.h"
#include "../qbrzip.h"

#include <QString>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QHash>
#include <QStringList>
#include <QFileInfo>

#include <QDebug>

/*
 * Format description: http://www.fictionbook.org/index.php/FictionBook_3.0_beta_-_%D0%BA%D1%80%D0%B0%D1%82%D0%BA%D0%BE%D0%B5_%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5
 * Many ideas found at: https://github.com/gribuser/FB3
 */

qbrformatfb3::qbrformatfb3()
{

}

bool qbrformatfb3::loadFile(QString fileName, QByteArray fileData)
{
    QRegExp rx("\\.fb3$", Qt::CaseInsensitive);
    if (rx.indexIn(fileName) < 0)
    {
        return false;
    }

    if (!isZipFile(fileData))
    {
        return false;
    }

    return parseFile(fileData);
}

QString qbrformatfb3::parseFB3TextFromNode(QDomNode xmlNode)
{
    if (xmlNode.isText())
    {
        return xmlNode.nodeValue();
    }

    QString rv = QString("");
    for (int i = 0; xmlNode.hasChildNodes() && i < xmlNode.childNodes().count(); i++)
    {
        rv.append(parseFB3TextFromNode(xmlNode.childNodes().at(i)));
    }
    return rv;
}

QString qbrformatfb3::parseFB3Node(QDomNode xmlNode)
{
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

    QString rv;
    if (xmlNode.hasChildNodes())
    {
        for (int i = 0; i < xmlNode.childNodes().count(); i++)
        {
            QDomNode curXmlNode = xmlNode.childNodes().at(i);
            if (tag_to_class.value(curXmlNode.nodeName(), "") != "")
            {
                rv.append("<div class=\"");
                rv.append(tag_to_class.value(curXmlNode.nodeName()));
                rv.append("\">\n");
                rv.append(parseFB3Node(curXmlNode));
                rv.append("</div>\n");
            }
            else if (curXmlNode.nodeName() == "section")
            {
                if (curXmlNode.attributes().contains("id"))
                {
                    QString section_id = curXmlNode.attributes().namedItem("id").toAttr().value();
                    rv.append("<div class=\"doc_section\" id=\"" + section_id + "\">\n");
                }
                else
                {
                    rv.append("<div class=\"doc_section\">\n");
                }
                rv.append(parseFB3Node(curXmlNode));
                rv.append("</div>\n");
            }
            else if (base_tags.value(curXmlNode.nodeName(), "") != "")
            {
                rv.append("<" + base_tags.value(curXmlNode.nodeName()) + ">");
                rv.append(parseFB3Node(curXmlNode));
                rv.append("</" + base_tags.value(curXmlNode.nodeName()) + ">");
            }
            else if (curXmlNode.nodeName() == "img")
            {
                QString img_src_key = curXmlNode.attributes().namedItem("src").toAttr().value();
                rv.append("<p>[IMG=" + img_src_key + "]</p>\n");
            }
            else
            {
                qDebug() << curXmlNode.nodeName();
                rv.append(parseFB3Node(curXmlNode));
            }
        }
    }
    else
    {
        //qDebug() << xmlNode.nodeName();
        if (xmlNode.nodeName() == "xml")
        {
            // Nothing to do
        }
        else if (xmlNode.nodeName() == "br")
        {
            rv.append("<br />\n");
        }
        rv.append(parseFB3TextFromNode(xmlNode));
    }
    return rv;

}


bool qbrformatfb3::parseFile(QByteArray fileData)
{
    if (!unZip.setData(fileData))
    {
        return false;
    }

    QStringList zipEntryNames = unZip.getFileNameList();
    QStringList requiredFiles = {
        "[Content_Types].xml",
        "_rels/.rels",
    };

    for (int i = 0; i < requiredFiles.count(); i++)
    {
        if (!zipEntryNames.contains(requiredFiles.at(i), Qt::CaseInsensitive))
        {
            return false;
        }
    }

    QDomDocument* docContentType = new QDomDocument();
    if ( !docContentType->setContent( unZip.getFileData( zipEntryNames.filter("[Content_Types].xml", Qt::CaseInsensitive).at(0) ) ) )
    {
        return false;
    }

    // Default file name of body entry
    QString body_entry_name = "/fb3/body.xml";
    // Try to find bodies
    QDomNodeList fb3_overrides = docContentType->elementsByTagName("Override");
    for (int i = 0; i < fb3_overrides.count(); i++)
    {
        if (
                fb3_overrides.at(i).attributes().contains("PartName") &&
                fb3_overrides.at(i).attributes().contains("ContentType") &&
                fb3_overrides.at(i).attributes().namedItem("ContentType").toAttr().value() == "application/fb3-body+xml"
            )
        {
            body_entry_name = fb3_overrides.at(i).attributes().namedItem("PartName").toAttr().value();
            break;
        }
    }


    // Body rels file name
    QString body_rels_entry_name;
    QFileInfo body_entry_name_info(body_entry_name);

    body_rels_entry_name.append(body_entry_name_info.path());
    if (!body_rels_entry_name.endsWith("/"))
    {
        body_rels_entry_name.append("/");
    }
    body_rels_entry_name.append("rels/");
    body_rels_entry_name.append(body_entry_name_info.fileName());
    body_rels_entry_name.append(".rels");


    // Try to load body
    QByteArray fb3_body_data = unZip.getFileData(body_entry_name);
    if (fb3_body_data == NULL && body_entry_name.startsWith("/"))
    {
        fb3_body_data = unZip.getFileData(body_entry_name.right(body_entry_name.length()-1));
    }
    if (fb3_body_data == NULL)
    {
        return false;
    }
    QDomDocument* bodyXml = new QDomDocument();
    if (!bodyXml->setContent(fb3_body_data))
    {
        return false;
    }


    // Try to load body rels

    htmlData.append(qbrtemplate::header());
    htmlData.append("<div class=\"document_body\">\n");
    for (int i = 0; i < bodyXml->childNodes().count(); i++)
    {

        htmlData.append(parseFB3Node(bodyXml->childNodes().at(i)));
    }
    htmlData.append("</div>\n");
    htmlData.append(qbrtemplate::footer());

    return true;
}

QString qbrformatfb3::getHtml()
{
    return htmlData;
}
