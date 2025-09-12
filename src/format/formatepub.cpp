#include "formatepub.h"

#include <QDomDocument>
#include <QDir>
#include <QUrl>


/**
 * EPub-parser. Based on https://www.w3.org/TR/epub-33
 */

FormatEPub::FormatEPub() = default;

bool FormatEPub::loadFile(QString fileName, QByteArray fileData, qbrunzip *zipData) {
    (void)fileName;

    bookInfo.metadata.clear();
    bookInfo.metadata.FileFormat = getFormatTitle();

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

QString FormatEPub::getFormatTitle()
{
    return "Electronic Publication (EPUB)";
}


QBRBook FormatEPub::getBook() {
    return bookInfo;
}

bool FormatEPub::needUnzip() { return true; }

bool FormatEPub::isValidFile(const qbrunzip *zipData) {
    if (!zipData->getFileData("mimetype").contains("application/epub+zip")) {
        return false;
    }

    if (!zipData->fileExists("META-INF/container.xml")) {
        return false;
    }

    return true;
}

QStringList FormatEPub::getRootFiles(const qbrunzip *zipData) {
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

QStringList FormatEPub::getEncryptedFiles(const qbrunzip *zipData) {
    QStringList encryptedFiles;
    if (!zipData->fileExists("META-INF/encryption.xml")) {
        return encryptedFiles;
    }

    qDebug() << "File partialy or fully encrypted!";

    QDomDocument containerXml;
    QString containerXmlErrorMsg;

    if (!containerXml.setContent(zipData->getFileData("META-INF/encryption.xml"), true, &containerXmlErrorMsg)) {
        qDebug() << "Can't read META-INF/encryption.xml: " << containerXmlErrorMsg;
    }
    else {
        QDomElement encryptedData = containerXml.firstChildElement("encryption").firstChildElement("EncryptedData");
        while (!encryptedData.isNull()) {
            QDomElement cipherReference = encryptedData.firstChildElement("CipherData").firstChildElement("CipherReference");
            if (cipherReference.hasAttribute("URI")) {
                QString encryptedFile = cipherReference.attribute("URI", "");
                if (encryptedFile.length() > 0) {
                    qDebug() << "Found encrypted object:" << encryptedFile;
                    encryptedFiles.append(encryptedFile);
                }
            }
            encryptedData = encryptedData.nextSiblingElement("EncryptedData");
        }
    }

    return encryptedFiles;
}

/**
 * @TODO: Rewrite to correspond part 4.2.4 of epub specification.
 * More info: https://www.w3.org/TR/epub-33/#sec-file-names-to-path-names
 */
QString FormatEPub::expandFileName(const QString& baseFileName, QString expandableFileName) {
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

QString FormatEPub::prepareLink(const QString& baseFileName, QString link) {
    QUrl linkUrl(link);
    if (linkUrl.scheme().compare("") != 0) {
        return link;
    }
    else if (linkUrl.hasFragment()) {
        return QString("#%1").arg(linkUrl.fragment());
    }

    return QString("#file_%1").arg(expandFileName(baseFileName, link));

}

QString FormatEPub::prepareDataLink(const qbrunzip *zipData, QString dataFileName, const QStringList& encryptedFiles) const
{
    // Encrypted file
    if (encryptedFiles.contains(dataFileName, Qt::CaseInsensitive)) {
        return EMPTYGIF;
    }

    QUrl dataFileUrl(dataFileName);

    // External web link
    if (QStringList({"http", "https"}).contains(dataFileUrl.scheme(), Qt::CaseInsensitive)) {
        return dataFileName;
    }

    // External non web link
    if (dataFileUrl.scheme().compare("") != 0) {
        return EMPTYGIF;
    }

    // Internal image
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
    else {
        return EMPTYGIF;
    }

    return QString("data:%1;base64,%2").arg(contentType, zipData->getFileData(dataFileName).toBase64());
}

QDomNode FormatEPub::processXHTMLNode(qbrunzip *zipData, const QString& xHTMLFileName, const QDomNode& currentNode, const QStringList& encryptedFiles) {
    //qDebug() << "Type: " << currentNode.nodeType() << ", Name: " << currentNode.nodeName() << ", Value: " << currentNode.nodeValue();

    const QList<QString> allowedTags = {
        "ul", "li",
        "p", "b", "i", "u", "s", "span", "pre", "strong", "em",
        "blockquote", "sub", "sup",
        "br", "hr",
        "a","img",
        "table", "tr", "th", "td","colgroup", "col", "thead", "tbody"
    };

    const QList<QString> allowedAttributes = {"id", "name"};

    const QHash<QString, QString> tagToClass = {
        {"body",     "document_body"},
        {"h1",       "doc_title"},
        {"h2",       "doc_subtitle"},
        {"h3",       "doc_subsubtitle"},
        {"h4",       "doc_subsubtitle_h4"},
        {"h5",       "doc_subsubtitle_h5"},
    };

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

            QDomElement returnValue = templateCreateElement(returnTagName);

            if (tagToClass.contains(currentNodeTag)) {
                returnValue.setAttribute("class", tagToClass.value(currentNodeTag));
            }

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
                    QString imgSrcFull = expandFileName(xHTMLFileName, imgSrc);
                    returnValue.setAttribute("src", prepareDataLink(zipData, imgSrcFull, encryptedFiles));
                }
            }

            for (int i = 0; (currentNode.hasAttributes() && i < allowedAttributes.count()); i++) {
                const QString& attrName = allowedAttributes.at(i);
                if (currentNode.attributes().contains(attrName)) {
                    returnValue.setAttribute(attrName, currentNode.attributes().namedItem(attrName).nodeValue());
                }
            }

            if (currentNode.hasChildNodes()) {
                for (int i = 0; i < currentNode.childNodes().count(); i++) {
                    QDomNode localNode = currentNode.childNodes().at(i);

                    returnValue.appendChild(processXHTMLNode(zipData, xHTMLFileName, localNode, encryptedFiles));
                }
            }
            // Dirty hack. But without it we have broken xHTML in some cases
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

bool FormatEPub::processXHTMLFile(QDomNode *xHTMLFileData, qbrunzip *zipData, const QString& xHTMLFileName, const QStringList& encryptedFiles) {
    if (encryptedFiles.contains(xHTMLFileName, Qt::CaseInsensitive)) {
        return false;
    }

    QDomDocument xHTMLFile;
    QString xHTMLErrorMsg;
    if (!xHTMLFile.setContent(zipData->getFileData(xHTMLFileName), true, &xHTMLErrorMsg)) {
        qDebug() << "Can't parse " << xHTMLFileName << ": " << xHTMLErrorMsg;
        return false;
    }

    QDomNodeList docBodies = xHTMLFile.elementsByTagName("body");

    QDomElement processResult = templateCreateElement("div");
    processResult.setAttribute("id", QString("file_%1").arg(xHTMLFileName.toHtmlEscaped()));
    for (int i = 0; i < docBodies.length(); i++) {
        QDomNode convertedNode = processXHTMLNode(zipData, xHTMLFileName, docBodies.at(i), encryptedFiles);
        processResult.appendChild(convertedNode);
    }

    xHTMLFileData->appendChild(processResult);

    return true;
}

void FormatEPub::processRootFileMetadata(const qbrunzip *zipData, const QString& rootFileName, const QDomDocument *rootFileXml, const QMap<QString,QDomElement> *manifestMap, const QStringList& encryptedFiles) {

    QDomElement metadataNode = rootFileXml->firstChildElement("package").firstChildElement("metadata");
    if (metadataNode.isNull()) {
        return;
    }

    QDomElement titleNode = metadataNode.firstChildElement("title");
    if (bookInfo.metadata.Title.compare("") == 0 && !titleNode.isNull()) {
        bookInfo.metadata.Title = titleNode.text();
    }

    QDomElement creatorNode = metadataNode.firstChildElement("creator");
    if (bookInfo.metadata.Author.compare("") == 0 && !creatorNode.isNull()) {
        bookInfo.metadata.Author = creatorNode.text();
    }

    // Cover imagw. Epub-2 Way
    QDomElement metaNode = metadataNode.firstChildElement("meta");
    while (!metaNode.isNull()) {
        if (metaNode.hasAttribute("name") && metaNode.attribute("name").compare("cover") == 0) {
            if (metaNode.hasAttribute("content")) {
                QString coverItemId = metaNode.attribute("content");
                if (manifestMap->contains(coverItemId)) {
                    QString coverImageName = expandFileName(rootFileName, manifestMap->value(coverItemId).attribute("href"));
                    if (!encryptedFiles.contains(coverImageName)) {
                        bookInfo.metadata.Cover.loadFromData(zipData->getFileData(coverImageName));
                    }
                }
                break;
            }
        }
        metaNode = metaNode.nextSiblingElement("meta");
    }

    // Cover imagw. Epub-3 Way
    if (bookInfo.metadata.Cover.isNull()) {
        for (int i = 0; i < manifestMap->values().count(); i++) {
            QDomElement manifestItem = manifestMap->values().at(i);
            if (manifestItem.attribute("properties", "").compare("cover-image") == 0) {
                QString coverImageName = expandFileName(rootFileName, manifestItem.attribute("href"));
                bookInfo.metadata.Cover.loadFromData(zipData->getFileData(coverImageName));
            }
        }
    }
}

bool FormatEPub::processRootFile(QDomNode *returnValue, qbrunzip *zipData, const QString& rootFileName, const QStringList& encryptedFiles) {
    QDomDocument rootFileXml;
    QString rootFileXmlErrorMsg;

    if (encryptedFiles.contains(rootFileName, Qt::CaseInsensitive)) {
        return false;
    }

    if (!rootFileXml.setContent(zipData->getFileData(rootFileName), true, &rootFileXmlErrorMsg)) {
        qDebug() << "Can't read rootfile: " << rootFileName << "Error: " << rootFileXmlErrorMsg;
        return false;
    }

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

                if (!processXHTMLFile(returnValue, zipData, expandFileName(rootFileName, manifestItemFileName), encryptedFiles)) {
                    return false;
                }
            }
        }
        spineItem = spineItem.nextSiblingElement("itemref");
    }

    processRootFileMetadata(zipData, rootFileName, &rootFileXml, &manifestMap, encryptedFiles);

    return true;
}

bool FormatEPub::parseFile(qbrunzip *zipData) {
    // Load META-INF/container.xml and get list of rootfiles
    QStringList rootFiles = getRootFiles(zipData);

    if (rootFiles.count() == 0) {
        return false;
    }

    QStringList encryptedFiles = getEncryptedFiles(zipData);

    templateInit();

    // Process rootfiles
    for (int i = 0; i < rootFiles.count(); i++) {
        QDomElement rootFileData = templateCreateElement("div");
        rootFileData.setAttribute("id", QString("rootfile_%1").arg(i));
        if (!processRootFile(&rootFileData, zipData, rootFiles.at(i), encryptedFiles)) {
            return false;
        }
        templateBodyAppend(rootFileData);
    }

    templateSetMeta(bookInfo.metadata);
    bookInfo.html += templateAsString();

    return true;
}
