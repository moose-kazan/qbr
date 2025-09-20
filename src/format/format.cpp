#include "format.h"

#include "../libs/settings.h"

#include <QDebug>
#include <QFile>
#include <QUrl>

Format::Format() = default;

bool Format::loadFile(QString fileName, QByteArray fileData, qbrunzip *zipData) {
    (void)fileName;
    (void)fileData;
    (void)zipData;
    return false;
};

QBRBook Format::getBook() {
    return QBRBook{};
};

QStringList Format::getExtensions() {
    return {};
};

QString Format::getFormatTitle()
{
    return tr("Unknown file format");
}


bool Format::needUnzip() {
    return false;
};


bool Format::isZipFile(const QByteArray& data) {
    constexpr char zipSignature[] = {0x50, 0x4B, 0x03, 0x04};
    return data.startsWith(QByteArrayView(zipSignature));
}

void Format::templateInit()
{
    templateDoc.clear();
    templateDoc = QDomDocument("html");

    QDomElement rootNode = templateCreateElement("html");
    rootNode.setAttribute("xmlns", "http://www.w3.org/1999/xhtml");
    // <head>
    QDomElement nodeHead = templateCreateElement("head");
    nodeHead.appendChild(templateCreateElement("title"));

    // <meta charset="utf-8">
    QDomElement nodeMetaCharset = templateCreateElement("meta");
    nodeMetaCharset.setAttribute("charset", "utf-8");
    nodeHead.appendChild(nodeMetaCharset);

    // <style>
    QDomElement nodeCSSDefault = templateCreateElement("style");
    nodeCSSDefault.appendChild(QDomDocument().createTextNode(
            templateLoadCSS(":/res/style.css")
        ));
    // </style>
    nodeHead.appendChild(nodeCSSDefault);

    if (Settings::getCustomStyleEnabled() && Settings::getCustomStyleUrl() != "")
    {
        QDomElement nodeStyle = templateCreateElement("style");

        nodeStyle.appendChild(QDomDocument().createTextNode(
            templateLoadCSS(QUrl(Settings::getCustomStyleUrl()).toLocalFile())
            ));
        nodeHead.appendChild(nodeStyle);
    }

    // </head>
    rootNode.appendChild(nodeHead);

    // <body></body>
    const QDomElement nodeBody = templateCreateElement("body");
    rootNode.appendChild(nodeBody);

    templateDoc.appendChild(rootNode);
    //qDebug() << templateDoc.toString();
}

void Format::templateBodyAppend(const QDomNode& bodyNode) const
{
    templateDoc.firstChildElement("html")
        .firstChildElement("body")
        .appendChild(bodyNode);
}

void Format::templateSetMeta(const qbrbookinfo& metadata) const
{
    QString title;
    if (!metadata.Author.isEmpty() && !metadata.Title.isEmpty()) {
        title = QString("%1 - %2").arg(metadata.Author, metadata.Title);
    } else if (!metadata.Author.isEmpty()){
        title = metadata.Author;
    } else if (!metadata.Title.isEmpty()) {
        title = metadata.Title;
    }
    templateDoc.firstChildElement("html")
        .firstChildElement("head")
        .firstChildElement("title")
        .appendChild(QDomDocument().createTextNode(title));
}

QString Format::templateAsString() const
{
    if (!templateDoc.isNull())
    {
        return templateDoc.toString();
    }
    return "";
}

QDomElement Format::templateCreateElement(const QString& tagName)
{
    QDomElement element = QDomDocument().createElement(tagName);
    element.appendChild(QDomDocument().createTextNode("\n"));
    return element;
}

QString Format::templateLoadCSS(const QString& fileName)
{
    QFile styleSheetFile(fileName);
    if (!styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open file " << fileName;
        return "";
    }
    QTextStream styleSheetStream(&styleSheetFile);
    QString styleSheet = styleSheetStream.readAll();
    styleSheetFile.close();

    return styleSheet;
}
