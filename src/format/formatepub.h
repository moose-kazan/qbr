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

private:
    QBRBook bookInfo;
    bool isValidFile(qbrzip *zipData);
    QStringList getRootFiles(qbrzip *zipData);
    QString expandFileName(QString baseFileName, QString expandableFileName);
    QString prepareLink(QString baseFileName, QString link);
    QString prepareDataLink(qbrzip *zipData, QString dataFileName);
    QDomNode processXHTMLNode(qbrzip *zipData, QString xHTMLFileName, QDomNode currentNode);
    QString processXHTMLFile(qbrzip *zipData, QString xHTMLFileName);
    void processRootFileMetadata(qbrzip *zipData, QString rootFileName, QDomDocument *rootFileXml, QMap<QString,QDomElement> *manifestMap);
    QString processRootFile(qbrzip *zipData, QString rootFileName);
    bool parseFile(qbrzip *zipData);
};

#endif // FORMATEPUB_H
