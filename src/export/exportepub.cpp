#include "exportepub.h"

#include <QBuffer>
#include <qdatetime.h>

#include "../libs/qbrzip.h"

#include <QDomDocument>
#include <QUuid>

ExportEPub::ExportEPub()
{
    zipWriter = new qbrzip();
};

QString ExportEPub::getFilter() {
    return QString(tr("EPub files (%1)")).arg("*.epub");
}

void ExportEPub::setData(QBRBook *book) {
    this->book = book;;
}

bool ExportEPub::save(const QString fileName)
{
    QMap<QString, QByteArray> images;
    QDomDocument htmlDocument = QDomDocument();
    htmlDocument.setContent(book->html.toUtf8());
    const QDomNodeList imagesNodes = htmlDocument.elementsByTagName("img");
    for (int i = 0; i < imagesNodes.length(); i++)
    {
        QString imageName;
        QStringList imageExtensions = {"png", "gif", "svg", "jpeg", "webp"};
        for (int j = 0; j < imageExtensions.length(); j++)
        {
            QString imgSrcTpl = QString("data:image/%1;base64,").arg(imageExtensions[j]);
            if (imagesNodes.at(i).toElement().attribute("src").startsWith(imgSrcTpl))
            {
                imageName = QString("image%1.%2")
                    .arg(i + 1, 4, 10, QChar('0'))
                    .arg(imageExtensions[j]);
                images[imageName] = QByteArray::fromBase64(imagesNodes.at(i).toElement().attribute("src").mid(imgSrcTpl.length()).toUtf8());
                imagesNodes.at(i).toElement().setAttribute("src", imageName);
                continue;
            }
        }
    }

    zipWriter->newFile();
    zipWriter->addItem({"mimetype", "application/epub+zip", zipItem::METHOD_STORE});
    zipWriter->addItem({"META-INF/container.xml", prepareContainerXml(), zipItem::METHOD_STORE});
    zipWriter->addItem({"content.opf", prepareContentOpf(images.keys()), zipItem::METHOD_STORE });
    zipWriter->addItem({"nav.xhtml", prepareNavXhtml(), zipItem::METHOD_DEFAULT });
    zipWriter->addItem({"index.xhtml", htmlDocument.toString().toUtf8(), zipItem::METHOD_DEFAULT });
    if (!book->metadata.Cover.isNull())
    {
        QByteArray coverImage;
        QBuffer coverImageBuffer(&coverImage);
        coverImageBuffer.open(QIODevice::WriteOnly);
        book->metadata.Cover.save(&coverImageBuffer, "PNG");
        coverImageBuffer.close();
        zipWriter->addItem({"cover.png", coverImage, zipItem::METHOD_DEFAULT});
    }
    for (int i = 0; i < images.size(); i++)
    {
        zipWriter->addItem({
            images.keys().at(i),
            images.values().at(i),
            zipItem::METHOD_DEFAULT });
    }

    return zipWriter->save(fileName);
}

QByteArray ExportEPub::prepareContainerXml()
{
    return QString(R"(<?xml version="1.0"?>
<container version="1.0" xmlns="urn:oasis:names:tc:opendocument:xmlns:container">
   <rootfiles>
      <rootfile full-path="content.opf" media-type="application/oebps-package+xml"/>
   </rootfiles>
</container>
)").toUtf8();
}

QByteArray ExportEPub::prepareContentOpf(const QStringList& images) const
{
    const auto contentOpfTemplate = QString(R"(<?xml version="1.0" encoding="UTF-8"?>
<package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="pub-id">
  <metadata xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/">
    <dc:title></dc:title>
    <dc:identifier id="pub-id"></dc:identifier>
    <dc:language>en</dc:language>
    <meta property="dcterms:modified"></meta>
  </metadata>
  <manifest>
    <item id="nav.xhtml" properties="nav" href="nav.xhtml" media-type="application/xhtml+xml"/>
    <item id="index.xhtml" href="index.xhtml" media-type="application/xhtml+xml"/>
  </manifest>
  <spine>
    <itemref idref="index.xhtml" />
  </spine>
</package>)");
    auto contentOpf = QDomDocument();
    contentOpf.setContent(contentOpfTemplate);
    // Title
    QString title = book->metadata.Title;
    if (title.isEmpty())
    {
        title = tr("Untitled");
    }
    contentOpf.firstChildElement("package")
        .firstChildElement("metadata")
        .firstChildElement("dc:title")
        .appendChild(QDomDocument().createTextNode(title));
    // UUID
    const QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    contentOpf.firstChildElement("package")
        .firstChildElement("metadata")
        .firstChildElement("dc:identifier")
        .appendChild(QDomDocument().createTextNode(uuid));
    // meta modified
    contentOpf.firstChildElement("package")
        .firstChildElement("metadata")
        .firstChildElement("meta")
        .appendChild(QDomDocument().createTextNode(QDateTime::currentDateTimeUtc().toString(Qt::ISODate)));

    if (!book->metadata.Cover.isNull())
    {
        QDomElement metaCoverImageNode = QDomDocument().createElement("meta");
        metaCoverImageNode.setAttribute("content", "cover");
        metaCoverImageNode.setAttribute("name", "cover");

        contentOpf.firstChildElement("package")
        .firstChildElement("metadata")
        .appendChild(metaCoverImageNode);

        QDomElement manifestCoverImageNode = QDomDocument().createElement("item");
        manifestCoverImageNode.setAttribute("href", "cover.png");
        manifestCoverImageNode.setAttribute("id", "cover");
        manifestCoverImageNode.setAttribute("media-type", "image/png");

        contentOpf.firstChildElement("package")
        .firstChildElement("manifest")
        .appendChild(manifestCoverImageNode);
    }

    for (const auto & image : images)
    {
        QDomElement itemNode = QDomDocument().createElement("item");
        itemNode.setAttribute("href", image);
        itemNode.setAttribute("id", image.mid(0,image.indexOf('.')));
        itemNode.setAttribute("media-type", QString("image/%1").arg(image.mid(image.indexOf('.')+1)));

        contentOpf.firstChildElement("package")
        .firstChildElement("manifest")
        .appendChild(itemNode);
    }

    return contentOpf.toByteArray();
}

QByteArray ExportEPub::prepareNavXhtml()
{
    const auto navXhtml = QString(R"(<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>Table of Contents</title>
</head>
<body>
<nav epub:type="toc" xmlns:epub="http://www.idpf.org/2007/ops">
    <ol>
        <li><a href="index.xhtml">Read book</a></li>
    </ol>
</nav>
</body>
</html>)");
    return navXhtml.toUtf8();
}