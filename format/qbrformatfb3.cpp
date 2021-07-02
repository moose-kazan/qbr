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
#include <QDebug>

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
    QString rv;
    if (xmlNode.hasChildNodes())
    {
        QHash<QString, QString> base_tags;
        base_tags.insert("strong", "strong");
        base_tags.insert("p", "p");
        base_tags.insert("em", "em");
        base_tags.insert("pre", "pre");
        base_tags.insert("sub", "sub");
        base_tags.insert("sup", "sup");
        base_tags.insert("s", "s");
        base_tags.insert("u", "u");
        base_tags.insert("b", "b");
        base_tags.insert("i", "i");
        base_tags.insert("blockquote", "blockquote");
        base_tags.insert("div", "div");

        for (int i = 0; i < xmlNode.childNodes().count(); i++)
        {
            QDomNode curXmlNode = xmlNode.childNodes().at(i);
            if (curXmlNode.nodeName() == "title")
            {
                rv.append("<div class=\"doc_title\">\n");
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
                //qDebug() << curXmlNode.attributes().namedItem("src").toAttr().value();
            }
            else
            {
                //qDebug() << curXmlNode.nodeName();
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

QString qbrformatfb3::parseFB3Body(QString bodyName)
{
    QByteArray fb3_body_data = unZip.getFileData(bodyName);
    if (fb3_body_data == NULL && bodyName.startsWith("/"))
    {
        fb3_body_data = unZip.getFileData(bodyName.right(bodyName.length()-1));
    }
    if (fb3_body_data == NULL)
    {
        return "";
    }
    QDomDocument* bodyXml = new QDomDocument();
    if (!bodyXml->setContent(fb3_body_data))
    {
        return "";
    }

    QString rv;

    for (int i = 0; i < bodyXml->childNodes().count(); i++)
    {
        rv.append("<div class=\"document_body\">\n");
        rv.append(parseFB3Node(bodyXml->childNodes().at(i)));
        rv.append("</div>\n");
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
        "fb3/_rels/description.xml.rels"
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
    // Try to find bodies
    QDomNodeList fb3_overrides = docContentType->elementsByTagName("Override");
    QList<QString> fb3_bodies_names;
    for (int i = 0; i < fb3_overrides.count(); i++)
    {
        if (
                fb3_overrides.at(i).attributes().contains("PartName") &&
                fb3_overrides.at(i).attributes().contains("ContentType") &&
                fb3_overrides.at(i).attributes().namedItem("ContentType").toAttr().value() == "application/fb3-body+xml"
            )
        {
            fb3_bodies_names.append(fb3_overrides.at(i).attributes().namedItem("PartName").toAttr().value());
        }
    }

    // If no bodies found we will use default name
    if (fb3_bodies_names.count() < 0)
    {
        fb3_bodies_names.append("/fb3/body.xml");
    }

    htmlData.append(qbrtemplate::header());
    for (int i = 0; i < fb3_bodies_names.count(); i++)
    {
        htmlData.append(parseFB3Body(fb3_bodies_names.at(i)));
    }

    htmlData.append(qbrtemplate::footer());

    return true;
}

QString qbrformatfb3::getHtml()
{
    return htmlData;
}
