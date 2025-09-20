#include "exportepub.h"

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
    zipWriter->newFile();
    zipWriter->addItem({"mimetype", "application/epub+zip", zipItem::METHOD_STORE});
    zipWriter->addItem({"META-INF/container.xml", prepareContainerXml(), zipItem::METHOD_STORE});
    zipWriter->addItem({"content.opf", prepareContentOpf(), zipItem::METHOD_STORE });
    zipWriter->addItem({"nav.xhtml", prepareNavXhtml(), zipItem::METHOD_DEFAULT });
    zipWriter->addItem({"index.xhtml", book->html.toUtf8(), zipItem::METHOD_DEFAULT });
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

QByteArray ExportEPub::prepareContentOpf() const
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
    contentOpf.firstChildElement("package")
        .firstChildElement("metadata")
        .firstChildElement("dc:title")
        .appendChild(QDomDocument().createTextNode(book->metadata.Title));
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