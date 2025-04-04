#include "formatepub.h"

#include "../template.h"

#include <QDebug>
#include <QDomDocument>
#include <QFileInfo>
#include <QDir>
#include <QUrl>


/**
 * EPub-parser. Based on https://www.w3.org/TR/epub-33
 */

FormatEPub::FormatEPub() {}

bool FormatEPub::loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) {
    (void)fileName;

    bookInfo = {{}, ""};
    bookInfo.metadata.FileFormat = "Electronic Publication (EPUB)";

    if (!isZipFile(fileData)) {
        return false;
    }

    if (!zipData->isLoaded()) {
        return false;
    }

    if (!isValidFile(zipData)) {
        return false;
    }

    return parseFile(zipData);
}

QStringList FormatEPub::getExtensions() { return QStringList("epub"); }

QBRBook FormatEPub::getBook() {
    return bookInfo;
}

bool FormatEPub::needUnzip() { return true; }

bool FormatEPub::isValidFile(qbrzip *zipData) {
    if (!zipData->getFileData("mimetype").contains("application/epub+zip")) {
        return false;
    }

    if (!zipData->fileExists("META-INF/container.xml")) {
        return false;
    }

    if (zipData->fileExists("META-INF/encryption.xml")) {
        qDebug() << "ePub files with encryption doesn't supported";
        return false;
    }


    return true;
}

QStringList FormatEPub::getRootFiles(qbrzip *zipData) {
    QStringList rootFiles;

    QDomDocument containerXml;
    QString containerXmlErrorMsg;

    if (!containerXml.setContent(zipData->getFileData("META-INF/container.xml"), true, &containerXmlErrorMsg)) {
        qDebug() << "Can't read META-INF/container.xml: " << containerXmlErrorMsg;
    }
    else {
        QDomElement rootFileNode = containerXml.firstChildElement("container").firstChildElement("rootfiles").firstChildElement("rootfile");
        while (!rootFileNode.isNull()) {
            if (rootFileNode.attribute("media-type", "").compare("application/oebps-package+xml") == 0) {
                QString rootFilePath = rootFileNode.attribute("full-path", "");
                if (rootFilePath.length() > 0) {
                    rootFiles.append(rootFilePath);
                }
            }

            rootFileNode = rootFileNode.nextSiblingElement("rootfile");
        }
    }


    return rootFiles;
}

/**
 * @TODO: Rewrite to correspond part 4.2.4 of epub specification.
 * More info: https://www.w3.org/TR/epub-33/#sec-file-names-to-path-names
 */
QString FormatEPub::expandFileName(QString baseFileName, QString expandableFileName) {
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

QString FormatEPub::prepareLink(QString baseFileName, QString link) {
    (void)baseFileName;

    QUrl  linkUrl(link);
    if (linkUrl.scheme().compare("") != 0) {
        return link;
    }
    else if (linkUrl.hasFragment()) {
        return QString("#%1").arg(linkUrl.fragment());
    }

    return QString("#file_%1").arg(expandFileName(baseFileName, link));

}

QString FormatEPub::prepareDataLink(qbrzip *zipData, QString dataFileName) {
    QString contentType;
    if (dataFileName.endsWith(".png", Qt::CaseInsensitive)) {
        contentType = "image/png";
    }
    else if (dataFileName.endsWith(".gif", Qt::CaseInsensitive)) {
        contentType = "image/gif";
    }
    else if (dataFileName.endsWith(".webp", Qt::CaseInsensitive)) {
        contentType = "image/webp";
    }
    else if (dataFileName.endsWith(".jpg", Qt::CaseInsensitive) || dataFileName.endsWith(".jpeg", Qt::CaseInsensitive)) {
        contentType = "image/jpeg";
    }

    return QString("data:%1;base64,%2").arg("").arg(zipData->getFileData(dataFileName).toBase64());
}

QDomNode FormatEPub::processXHTMLNode(qbrzip *zipData, QString xHTMLFileName, QDomNode currentNode) {
    //qDebug() << "Type: " << currentNode.nodeType() << ", Name: " << currentNode.nodeName() << ", Value: " << currentNode.nodeValue();

    QList<QString> allowedTags = {
        "ul", "li",
        "p", "b", "i", "u", "s", "span", "pre", "strong", "em",
        "blockquote", "sub", "sup",
        "br", "hr",
        "a","img",
        "table", "tr", "th", "td","colgroup", "col", "thead", "tbody"
    };
    QList<QString> allowedAttributes = {"id", "name"};

    switch (currentNode.nodeType()) {
    case QDomNode::ElementNode:
        {

            QString returnTagName = "div";
            QString currentNodeTag = currentNode.nodeName().toLower();

            if (allowedTags.contains(currentNodeTag)) {
                returnTagName = currentNodeTag;
            }
            /*
            else {
                qDebug() << "nodeName" << currentNode.nodeName() << "nodeValue" << currentNode.nodeValue();
            }
            */

            QDomElement returnValue = QDomDocument().createElement(returnTagName);

            if (returnTagName.compare("a") == 0) {
                if (currentNode.attributes().contains("href")) {
                    QString aHref = prepareLink(xHTMLFileName, currentNode.attributes().namedItem("href").nodeValue());
                    returnValue.setAttribute("href", aHref);
                    returnValue.setAttribute("title", aHref);
                }
            }
            else if (returnTagName.compare("img") == 0) {
                if (currentNode.attributes().contains("src")) {
                    QString imgSrc = currentNode.attributes().namedItem("src").nodeValue();
                    returnValue.setAttribute("src", prepareDataLink(zipData, expandFileName(xHTMLFileName, imgSrc)));
                }
            }

            if (currentNodeTag.compare("body") == 0) {
                returnValue.setAttribute("class", "document_body");
            }
            else if (currentNodeTag.compare("h1") == 0) {
                returnValue.setAttribute("class", "doc_title");
            }
            else if (currentNodeTag.compare("h2") == 0) {
                returnValue.setAttribute("class", "doc_subtitle");
            }
            else if (currentNodeTag.compare("h3") == 0) {
                returnValue.setAttribute("class", "doc_subsubtitle");
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

                    returnValue.appendChild(processXHTMLNode(zipData, xHTMLFileName, localNode));
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

QString FormatEPub::processXHTMLFile(qbrzip *zipData, QString xHTMLFileName) {
    QDomDocument xHTMLFile;
    QString xHTMLErrorMsg;
    if (!xHTMLFile.setContent(zipData->getFileData(xHTMLFileName), true, &xHTMLErrorMsg)) {
        qDebug() << "Can't parse " << xHTMLFileName << ": " << xHTMLErrorMsg;
        return "";
    }

    QString processResult;
    QDomNodeList docBodies = xHTMLFile.elementsByTagName("body");

    for (int i = 0; i < docBodies.length(); i++) {
        QDomNode convertedNode = processXHTMLNode(zipData, xHTMLFileName, docBodies.at(i));
        QString nodeAsString;
        QTextStream nodeStream(&nodeAsString);
        convertedNode.save(nodeStream, 2);
        processResult.append(nodeAsString);
    }

    return QString("<div id=\"#file_%1\">%2</div>\n").arg(xHTMLFileName.toHtmlEscaped()).arg(processResult);
}

QString FormatEPub::processRootFile(qbrzip *zipData, QString rootFileName) {
    QDomDocument rootFileXml;
    QString rootFileXmlErrorMsg;

    if (!rootFileXml.setContent(zipData->getFileData(rootFileName), true, &rootFileXmlErrorMsg)) {
        qDebug() << "Can't read rootfile: " << rootFileName << "Error: " << rootFileXmlErrorMsg;
        return "";
    }

    QString returnValue;
    QMap<QString,QDomElement> manifestMap;

    QDomElement manifestItem = rootFileXml.firstChildElement("package").firstChildElement("manifest").firstChildElement("item");
    while (!manifestItem.isNull()) {
        QString manifestItemFileId = manifestItem.attribute("id", "");
        manifestMap[manifestItemFileId] = manifestItem;
        manifestItem = manifestItem.nextSiblingElement("item");
    }

    QDomElement spineItem = rootFileXml.firstChildElement("package").firstChildElement("spine").firstChildElement("itemref");
    while (!spineItem.isNull()) {
        QString manifestItemFileId = spineItem.attribute("idref", "");

        if (manifestMap.contains(manifestItemFileId)) {
            if (manifestMap.value(manifestItemFileId).attribute("media-type", "").compare("application/xhtml+xml") == 0) {
                QString manifestItemFileName = manifestMap.value(manifestItemFileId).attribute("href", "");

                QString xHTMLData = processXHTMLFile(zipData, expandFileName(rootFileName, manifestItemFileName));

                returnValue.append(xHTMLData);
            }
        }
        spineItem = spineItem.nextSiblingElement("itemref");
    }

    return returnValue;
}

bool FormatEPub::parseFile(qbrzip *zipData) {
    // Load META-INF/container.xml and get list of rootfiles
    QStringList rootFiles = getRootFiles(zipData);

    if (rootFiles.count() == 0) {
        return false;
    }

    bookInfo.html = Template::header();

    // Process rootfiles
    for (int i = 0; i < rootFiles.count(); i++) {
        bookInfo.html += QString("<div id=\"rootfile_%1\">%2</div>\n").arg(i).arg(processRootFile(zipData, rootFiles.at(i)));
    }

    bookInfo.html += Template::footer();

    return true;
}
