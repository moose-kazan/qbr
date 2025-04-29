#ifndef FORMATEPUB_H
#define FORMATEPUB_H

#include "format.h"
#include "../libs/qbrzip.h"
#include <QByteArray>
#include <QString>
#include <QDomNode>
#include <QMap>

class FormatEPub : public Format
{
public:
    FormatEPub();
    bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) override;
    QStringList getExtensions() override;
    QBRBook getBook() override;
    bool needUnzip() override;

    const QString EMPTYGIF = "data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7";

private:
    QBRBook bookInfo;
    bool isValidFile(qbrzip *zipData);
    QStringList getRootFiles(qbrzip *zipData);
    QStringList getEncryptedFiles(qbrzip *zipData);
    QString expandFileName(QString baseFileName, QString expandableFileName);
    QString prepareLink(QString baseFileName, QString link);
    QString prepareDataLink(qbrzip *zipData, QString dataFileName, QStringList encryptedFiles);
    QDomNode processXHTMLNode(qbrzip *zipData, QString xHTMLFileName, QDomNode currentNode, QStringList encryptedFiles);
    bool processXHTMLFile(QString *xHTMLFileData, qbrzip *zipData, QString xHTMLFileName, QStringList encryptedFiles);
    void processRootFileMetadata(qbrzip *zipData, QString rootFileName, QDomDocument *rootFileXml, QMap<QString,QDomElement> *manifestMap, QStringList encryptedFiles);
    bool processRootFile(QString *returnValue, qbrzip *zipData, QString rootFileName, QStringList encryptedFiles);
    bool parseFile(qbrzip *zipData);
};

#endif // FORMATEPUB_H
