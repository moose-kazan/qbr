#include "qbrformatfb2.h"
#include "../qbrtemplate.h"

#include <QString>
#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QHash>

#include <QDebug>

qbrformatfb2::qbrformatfb2()
{

}

QString qbrformatfb2::parseXmlTextFromNode(QDomNode xmlNode)
{
    if (xmlNode.isText())
    {
        return xmlNode.nodeValue();
    }

    QString rv = QString("");
    for (int i = 0; xmlNode.hasChildNodes() && i < xmlNode.childNodes().count(); i++)
    {
        rv.append(parseXmlTextFromNode(xmlNode.childNodes().at(i)));
    }
    return rv;
}

QString qbrformatfb2::parseXmlBody(QDomNode xmlNode, QHash<QString, QString> xmlImages)
{
    QString rv = QString("");
    for (int i = 0; i < xmlNode.childNodes().count(); i++)
    {
        QDomNode curXmlNode = xmlNode.childNodes().at(i);
        if (curXmlNode.childNodes().count() == 0)
        {
            if (curXmlNode.nodeName() == "#text")
            {
                rv.append(parseXmlTextFromNode(curXmlNode));
            }
            else if (curXmlNode.nodeName() == "empty-line")
            {
                rv.append("<br />\n<br />\n");
            }
            else if (curXmlNode.nodeName() == "image")
            {
                QString img_src_key = QString("");
                if (curXmlNode.attributes().contains("xlink:href"))
                {
                    img_src_key = curXmlNode.attributes().namedItem("xlink:href").toAttr().value();
                }
                else if (curXmlNode.attributes().contains("l:href"))
                {
                    img_src_key = curXmlNode.attributes().namedItem("l:href").toAttr().value();
                }
                else if (curXmlNode.attributes().contains("href"))
                {
                    img_src_key = curXmlNode.attributes().namedItem("href").toAttr().value();
                }

                if (xmlImages.value(img_src_key, "") != "")
                {
                    rv.append("<img src=\"" + xmlImages.value(img_src_key, "") + "\" />");
                }
            }
        }
        else
        {
            QHash<QString, QString> base_tags;
            base_tags.insert("strong", "strong");
            base_tags.insert("p", "p");
            base_tags.insert("emphasis", "em");
            base_tags.insert("code", "pre");
            base_tags.insert("sub", "sub");
            base_tags.insert("sup", "sup");
            base_tags.insert("strikethrough", "s");
            base_tags.insert("cite", "blockquote");

            bool base_tag = false;
            for (int j = 0; j < base_tags.keys().count(); j++)
            {
                if (curXmlNode.nodeName() == base_tags.keys().value(j))
                {
                    base_tag = true;
                    QString html_tag = base_tags[base_tags.keys().value(j)];
                    rv.append("<" + html_tag + ">" + parseXmlBody(curXmlNode, xmlImages) + "</" + html_tag + ">\n");
                }
            }
            if (!base_tag)
            {
                if (curXmlNode.nodeName() == "title")
                {
                    rv.append("<div class=\"doc_title\">" + parseXmlBody(curXmlNode, xmlImages) + "</div>\n");
                }
                else if (curXmlNode.nodeName() == "subtitle")
                {
                    rv.append("<div class=\"doc_subtitle\">" + parseXmlBody(curXmlNode, xmlImages) + "</div>\n");
                }
                else if (curXmlNode.nodeName() == "stanza")
                {
                    rv.append("<div class=\"doc_poem\">" + parseXmlBody(curXmlNode, xmlImages) + "</div>\n");
                }
                else if (curXmlNode.nodeName() == "v")
                {
                    rv.append(parseXmlBody(curXmlNode, xmlImages) + "<br />\n");
                }
                else if (curXmlNode.nodeName() == "section")
                {
                    if (curXmlNode.attributes().contains("id"))
                    {
                        rv.append("<div class=\"doc_section\" id=\"");
                        rv.append(curXmlNode.attributes().namedItem("id").toAttr().value());
                        rv.append("\">");
                        rv.append(parseXmlBody(curXmlNode, xmlImages));
                        rv.append("</div>\n");
                    }
                    else
                    {
                        rv.append("<div class=\"doc_section\">" + parseXmlBody(curXmlNode, xmlImages) + "</div>\n");
                    }
                }
                else if (curXmlNode.nodeName() == "a")
                {
                    QString a_type = "";
                    if (curXmlNode.attributes().contains("type"))
                    {
                        a_type = curXmlNode.attributes().namedItem("type").toAttr().value();
                    }
                    QString a_href = "";
                    if (curXmlNode.attributes().contains("l:href"))
                    {
                        a_href = curXmlNode.attributes().namedItem("l:href").toAttr().value();
                    }
                    else if (curXmlNode.attributes().contains("xlink:href"))
                    {
                        a_href = curXmlNode.attributes().namedItem("xlink:href").toAttr().value();
                    }
                    else if (curXmlNode.attributes().contains("href"))
                    {
                        a_href = curXmlNode.attributes().namedItem("href").toAttr().value();
                    }
                    if (a_type == "note" && a_href != "")
                    {
                        rv.append("<a class=\"doc_note_link\" href=\"" + a_href + "\">");
                        rv.append(parseXmlBody(curXmlNode, xmlImages));
                        rv.append("</a>\n");
                    }
                    else
                    {
                        rv.append(parseXmlBody(curXmlNode, xmlImages));
                    }
                }
                else
                {
                    rv.append(parseXmlBody(curXmlNode, xmlImages));
                }
            }
        }
    }
    return rv;
}

bool qbrformatfb2::parseXml(QByteArray fileData)
{

    QDomDocument* parserXml = new QDomDocument();
    QString paserXmlErrorMsg;
    int parserXmlErrorLine;
    int parserXmlErrorColumn;
    if (!parserXml->setContent(fileData, true, &paserXmlErrorMsg, &parserXmlErrorLine, &parserXmlErrorColumn)) {
        qDebug() << "Error message: " << paserXmlErrorMsg << " at line " << parserXmlErrorLine << ", column: " << parserXmlErrorColumn;
        return false;
    }


    QHash<QString, QString> fb2_images;
    QDomNodeList fb2_binaries = parserXml->elementsByTagName("binary");
    for (int i = 0; i < fb2_binaries.count(); i++)
    {
        QDomNode fb2_binary = fb2_binaries.at(i);
        if (fb2_binary.attributes().contains("id") && fb2_binary.attributes().contains("content-type"))
        {
            QString img_key = QString("#");
            img_key.append(fb2_binary.attributes().namedItem("id").toAttr().value());

            QString img_val = QString("data:");
            img_val.append(fb2_binary.attributes().namedItem("content-type").toAttr().value());
            img_val.append(";base64,");
            img_val.append(parseXmlTextFromNode(fb2_binary).replace("\n", "").replace("\r", ""));

            fb2_images.insert(img_key, img_val);
        }
    }

    htmlData.append(qbrtemplate::header());
    QDomNodeList fb2_bodies = parserXml->elementsByTagName("body");
    if (fb2_bodies.count() < 1) {
        return false;
    }

    for (int i = 0; i < fb2_bodies.count(); i++)
    {
        QDomNode fb2_body = fb2_bodies.at(i);
        QString fb2_body_line = "";
        if (fb2_body.attributes().contains("name"))
        {
            fb2_body_line.append("<div class=\"document_body\" name=\"");
            fb2_body_line.append(fb2_body.attributes().namedItem("name").toAttr().value());
            fb2_body_line.append("\">");
            fb2_body_line.append(parseXmlBody(fb2_body, fb2_images));
            fb2_body_line.append("</div>\n");
        }
        else
        {
            fb2_body_line.append("<div class=\"document_body\">");
            fb2_body_line.append(parseXmlBody(fb2_body, fb2_images));
            fb2_body_line.append("</div>\n");
        }
        htmlData.append(fb2_body_line);
    }
    htmlData.append(qbrtemplate::footer());

    return true;
}

bool qbrformatfb2::loadFile(QString fileName, QByteArray fileData)
{
    try
    {
        return parseXml(fileData);
    }
    catch (...)
    {
        return false;
    }

    return false;
}

QString qbrformatfb2::getHtml()
{
    return htmlData;
}
