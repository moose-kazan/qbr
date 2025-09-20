#include "formatfb3.h"

#include <QByteArray>
#include <QDebug>
#include <QDomDocument>
#include <QDir>
#include <QHash>
#include <QRegularExpression>
#include <QString>
#include <QUrl>

/*
 * Format description:
 * http://www.fictionbook.org/index.php/FictionBook_3.0_beta_-_%D0%BA%D1%80%D0%B0%D1%82%D0%BA%D0%BE%D0%B5_%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5
 * Many ideas found at: https://github.com/gribuser/FB3
 */

FormatFB3::FormatFB3() = default;

QStringList FormatFB3::getExtensions() { return QStringList("fb3"); }

QString FormatFB3::getFormatTitle()
{
    return "FictionBook 3";
}


bool FormatFB3::loadFile(const QString fileName, const QByteArray fileData, qbrunzip *zipData) {
    // reset data from previous file
    htmlData = "";
    bookInfo.clear();
    bookInfo.FileFormat = getFormatTitle();

    fb3ExtTypes.clear();

    if (!fileNameRegexp.match(fileName).hasMatch()) {
        return false;
    }

    if (!isZipFile(fileData)) {
        return false;
    }

    if (!zipData->isLoaded()) {
        return false;
    }

    const QStringList requiredFiles = {
        "[Content_Types].xml",
        "_rels/.rels",
    };

    for (int i = 0; i < requiredFiles.count(); i++) {
        if (!zipData->fileExists(requiredFiles.at(i))) {
            // qDebug() << "can't find file: " << requiredFiles.at(i);
            return false;
        }
    }

    return parseFile(zipData);
}

QString FormatFB3::expandFileName(const QString& baseFileName, QString expandableFileName) {
    // If absolute path
    if (expandableFileName.startsWith("/")) {
        return expandableFileName.remove(0, 1);
    }

    QString expandedFileName = QFileInfo(baseFileName).dir().filePath(expandableFileName);
    expandedFileName = QDir::cleanPath(expandedFileName);

    if (expandedFileName.startsWith("./")) {
        return expandedFileName.remove(0, 2);
    }

    return expandedFileName;
}

QString FormatFB3::getRelsFileName(const QString& baseFileName) {
    const QFileInfo fi(baseFileName);
    const QString relsFileName = fi.dir().dirName().append("/_rels/").append(fi.fileName().append(".rels"));

    return QDir::cleanPath(relsFileName);
}

void FormatFB3::parseMetadata(const qbrunzip *ZipData, const QString& entryName) {
    const QByteArray fb3_description_data(ZipData->getFileData(entryName));
    if (fb3_description_data.isNull()) {
        return;
    }

    QDomDocument descriptionXml;
    if (!descriptionXml.setContent(fb3_description_data)) {
        return;
    }

    if (const QDomElement nodeBookTitle = descriptionXml.firstChildElement("fb3-description").firstChildElement("title").firstChildElement("main"); !nodeBookTitle.isNull()) {
        bookInfo.Title = nodeBookTitle.text();
    }

    QDomNode relationsItem = descriptionXml.firstChildElement("fb3-description").firstChildElement("fb3-relations").firstChild();
    while (!relationsItem.isNull()) {
        if (QDomElement authorElement = relationsItem.toElement(); !authorElement.isNull() && authorElement.attribute("link", "").compare("author") == 0) {
            QString firstName = authorElement.firstChildElement("first-name").text();
            QString lastName = authorElement.firstChildElement("last-name").text();
            QString middleName = authorElement.firstChildElement("middle-name").text();
            QString nameTitle = authorElement.firstChildElement("title").text();

            if (!firstName.isEmpty() && !lastName.isEmpty()) {
                nameTitle = firstName;
                if (!middleName.isEmpty()) {
                    nameTitle.append(" ");
                    nameTitle.append(middleName);
                }
                nameTitle.append(" ");
                nameTitle.append(lastName);
            }

            if (bookInfo.Author.isEmpty()) {
                bookInfo.Author = nameTitle;
            }
            else {
                bookInfo.Author.append(", ");
                bookInfo.Author.append(nameTitle);
            }
        }

        relationsItem = relationsItem.nextSibling();
    }
}

QList<QDomElement> FormatFB3::parseRels(const qbrunzip *ZipData, const QString& entryName) {
    QList<QDomElement> relsData;

    // Try to load body rels file
    const QByteArray bodyRelsData(ZipData->getFileData(entryName));
    if (bodyRelsData.isNull()) {
        return relsData;
    }

    QDomDocument bodyRelsXml;
    if (!bodyRelsXml.setContent(bodyRelsData)) {
        return relsData;
    }

    // Load binaries
    QDomElement relationshipNode = bodyRelsXml.firstChildElement("Relationships").firstChildElement("Relationship");
    while (!relationshipNode.isNull()) {
        if (relationshipNode.hasAttribute("Id") && relationshipNode.hasAttribute("Target")) {
            relsData.append(relationshipNode.cloneNode().toElement());
        }

        relationshipNode = relationshipNode.nextSiblingElement("Relationship");
    }

    return relsData;
}

QDomNode FormatFB3::parseBodyNode(const QDomNode& currentNode, const QMap<QString,QString>& bodyBinaries) {
    const QHash<QString, QString> baseTags = {
        {"strong",        "strong"},
        {"p",             "p"},
        {"em0",           "em"},
        {"code",          "pre"},
        {"sub",           "sub"},
        {"sup",           "sup"},
        {"strikethrough", "s"},
        {"br",            "br"},
        {"a",             "a"},
        {"table",         "table"},
        {"tr",            "tr"},
        {"th",            "th"},
        {"td",            "td"},
        {"img",           "img"}
    };

    const QHash<QString, QString> tagToClass = {
        {"fb3-body",   "document_body"},
        {"title",      "doc_title"},
        {"subtitle",   "doc_subtitle"},
        {"stanza",     "doc_poem"},
        {"poem",       "doc_poem"},
        {"section",    "doc_section"},
        {"notebody",   "doc_note_body"},
        {"notes",      "doc_notes"},
        {"epigraph",   "doc_epigraph"},
        {"annotation", "doc_annotation"},
    };

    const QList<QString> allowedAttributes = {"id", "name"};

    switch (currentNode.nodeType()) {
    case QDomNode::ElementNode:
    {
        QString returnTagName = "div";
        const QString currentNodeTag = currentNode.nodeName().toLower();

        if (baseTags.contains(currentNodeTag)) {
            returnTagName = baseTags.value(currentNodeTag);
        }
        /*else {
            qDebug() << "nodeName" << currentNode.nodeName() << "nodeValue" << currentNode.nodeValue();
        }*/

        QDomElement returnValue = templateCreateElement(returnTagName);

        if (tagToClass.contains(currentNodeTag)) {
            returnValue.setAttribute("class", tagToClass.value(currentNodeTag));
        }

        if (returnTagName.compare("img") == 0) {
            if (currentNode.attributes().contains("src")) {
                const QString imgSrc = currentNode.attributes().namedItem("src").nodeValue();
                returnValue.setAttribute("src", bodyBinaries.value(imgSrc));
            }
        }
        else if (returnTagName.compare("a") == 0) {
            if (currentNode.attributes().contains("href")) {
                const QString aHref = currentNode.attributes().namedItem("href").nodeValue();
                //qDebug() << "a.href" << aHref;
                returnValue.setAttribute("href", aHref);
                returnValue.setAttribute("title", aHref);
            }
            if (currentNode.attributes().contains("type")) {
                if (const QString aType = currentNode.attributes().namedItem("type").nodeValue(); aType.compare("note") == 0) {
                    returnValue.setAttribute("class", "doc_note_link");
                }
            }
        }

        for (int i = 0; (currentNode.hasAttributes() && i < allowedAttributes.count()); i++) {
            if (const QString& attrName = allowedAttributes.at(i); currentNode.attributes().contains(attrName)) {
                returnValue.setAttribute(attrName, currentNode.attributes().namedItem(attrName).nodeValue());
            }
        }

        if (currentNode.hasChildNodes()) {
            for (int i = 0; i < currentNode.childNodes().count(); i++) {
                QDomNode localNode = currentNode.childNodes().at(i);

                returnValue.appendChild(parseBodyNode(localNode, bodyBinaries));
            }
        }
        // Dirty hack. But without it, we have broken xHTML in some cases
        else {
            returnValue.appendChild(QDomDocument().createTextNode(""));
        }

        return returnValue;
    }
    case QDomNode::TextNode:
        return currentNode.cloneNode();
    case QDomNode::EntityReferenceNode:
        return currentNode.cloneNode();
    default:
        //qDebug() << "nodeType" << currentNode.nodeType() << "nodeName" << currentNode.nodeName() << "nodeValue" << currentNode.nodeValue() << "count" << currentNode.childNodes().count();
        break;
    }

    return QDomDocument().createTextNode("");
}

QList<QDomNode> FormatFB3::parseBody(const qbrunzip* zipData, const QString& bodyEntryName) const
{
    const QList<QDomElement> relsData = parseRels(zipData, getRelsFileName(bodyEntryName));
    QMap<QString,QString> bodyBinaries;

    for (int i = 0; i < relsData.count(); i++) {
        QString nodeTarget = expandFileName(bodyEntryName, relsData.at(i).attribute("Target", ""));
        QString nodeId = relsData.at(i).attribute("Id", "");
        QString nodeTargetExtension = nodeTarget.section('.', -1).toLower();
        QString nodeContentType = fb3ExtTypes.value(nodeTargetExtension, "");

        bodyBinaries.insert(nodeId, QString("data:%1;base64,%2").arg(nodeContentType, zipData->getFileData(nodeTarget).toBase64()));
        //qDebug() << nodeId << nodeTargetExtension << nodeContentType << nodeTarget << bodyBinaries.value(nodeId).length();
    }

    QDomDocument xmlFile;
    QString xmlErrorMsg;
    if (!xmlFile.setContent(zipData->getFileData(bodyEntryName), true, &xmlErrorMsg)) {
        qDebug() << "Can't parse " << bodyEntryName << ": " << xmlErrorMsg;
        return {};
    }

    QList<QDomNode> processResult;
    const QDomNodeList docBodies = xmlFile.elementsByTagName("fb3-body");

    for (int i = 0; i < docBodies.length(); i++) {
        QDomNode convertedNode = parseBodyNode(docBodies.at(i), bodyBinaries);
        processResult.append(convertedNode);
    }


    return processResult;
}

bool FormatFB3::parseFile(const qbrunzip *ZipData) {
    const QByteArray docContentTypeData = ZipData->getFileData("[Content_Types].xml");
    if (docContentTypeData.isNull()) {
        // qDebug() << "can't extract [Content_Types].xml";
        return false;
    }

    QDomDocument docContentType;
    QString docContentTypeErrorMsg;
    if (!docContentType.setContent(docContentTypeData, true, &docContentTypeErrorMsg)) {
        qDebug() << "can't parse [Content_Types].xml" << docContentTypeErrorMsg;
        return false;
    }

    // Default file name of body entry
    QString bodyEntryName = "fb3/body.xml";

    // Default file name of description entry
    QString descriptionEntryName = "fb3/description.xml";

    // Extensions -> Content-TYpe
    fb3ExtTypes.clear();

    // Load content-types for extensions
    QDomElement contentTypeNode = docContentType.firstChildElement("Types").firstChildElement("Default");
    while (!contentTypeNode.isNull()) {
        if (contentTypeNode.hasAttribute("Extension") && contentTypeNode.hasAttribute("ContentType")) {
            fb3ExtTypes.insert(contentTypeNode.attribute("Extension"), contentTypeNode.attribute("ContentType"));
        }
        contentTypeNode = contentTypeNode.nextSiblingElement("Default");
    }

    // Try to find bodies and load overrides
    QDomElement overrideNode = docContentType.firstChildElement("Types").firstChildElement("Override");
    while (!overrideNode.isNull()) {
        if (overrideNode.hasAttribute("PartName") && overrideNode.hasAttribute("ContentType")) {
            const QString overridePartName = expandFileName("[Content_Types].xml", overrideNode.attribute("PartName"));
            if (overrideNode.attribute("ContentType", "").compare("application/fb3-body+xml") == 0) {
                bodyEntryName = overridePartName;
            }
            else if (overrideNode.attribute("ContentType", "").compare("application/fb3-description+xml") == 0) {
                descriptionEntryName = overridePartName;
            }
        }

        overrideNode = overrideNode.nextSiblingElement("Override");
    }

    const QList<QDomElement> relsData = parseRels(ZipData, "_rels/.rels");
    for (int i = 0; i < relsData.count(); i++) {
        QString nodeType = relsData.at(i).attribute("Type", "");
        const QString nodeTarget = relsData.at(i).attribute("Target", "");

        if (nodeType.compare(RELATION_THUMBNAIL) == 0) {
            bookInfo.Cover.loadFromData(ZipData->getFileData(nodeTarget));
        }
    }

    const QList<QDomNode> documentBody = parseBody(ZipData, bodyEntryName);
    templateInit();
    for (int i = 0; i < documentBody.count(); i++)
    {
        templateBodyAppend(documentBody.at(i));
    }

    htmlData = templateAsString();

    parseMetadata(ZipData, descriptionEntryName);

    return true;
}

QBRBook FormatFB3::getBook() { return QBRBook{bookInfo, htmlData}; }

bool FormatFB3::needUnzip() { return true; }
