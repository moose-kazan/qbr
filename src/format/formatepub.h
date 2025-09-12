#ifndef FORMATEPUB_H
#define FORMATEPUB_H

#include "format.h"
#include "../libs/qbrzip.h"
#include <QString>
#include <QDomNode>
#include <QMap>

class FormatEPub : public Format
{
public:
    FormatEPub();
    bool loadFile(QString fileName, QByteArray fileData, qbrzip *zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook getBook() override;
    bool needUnzip() override;

    const QString EMPTYGIF = "data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7";

private:
    QBRBook bookInfo;
    static bool isValidFile(const qbrzip *zipData);
    static QStringList getRootFiles(const qbrzip *zipData);
    static QStringList getEncryptedFiles(const qbrzip *zipData);
    static QString expandFileName(const QString& baseFileName, QString expandableFileName);
    static QString prepareLink(const QString& baseFileName, QString link);
    QString prepareDataLink(const qbrzip *zipData, QString dataFileName, const QStringList& encryptedFiles) const;
    QDomNode processXHTMLNode(qbrzip *zipData, const QString& xHTMLFileName, const QDomNode& currentNode, const QStringList& encryptedFiles);
    bool processXHTMLFile(QDomNode* xHTMLFileData, qbrzip* zipData, const QString& xHTMLFileName, const QStringList& encryptedFiles);
    void processRootFileMetadata(const qbrzip *zipData, const QString& rootFileName, const QDomDocument *rootFileXml, const QMap<QString,QDomElement> *manifestMap, const QStringList& encryptedFiles);
    bool processRootFile(QDomNode* returnValue, qbrzip* zipData, const QString& rootFileName, const QStringList& encryptedFiles);
    bool parseFile(qbrzip *zipData);
};

#endif // FORMATEPUB_H
