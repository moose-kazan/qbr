#ifndef FORMATEPUB_H
#define FORMATEPUB_H

#include "format.h"
#include "../libs/qbrunzip.h"
#include <QString>
#include <QDomNode>
#include <QMap>

class FormatEPub final : public Format
{
public:
    FormatEPub();
    bool loadFile(QString fileName, QByteArray fileData, qbrunzip *zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook getBook() override;
    bool needUnzip() override;

    const QString EMPTYGIF = "data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7";

private:
    QBRBook bookInfo;
    static bool isValidFile(const qbrunzip *zipData);
    static QStringList getRootFiles(const qbrunzip *zipData);
    static QStringList getEncryptedFiles(const qbrunzip *zipData);
    static QString expandFileName(const QString& baseFileName, QString expandableFileName);
    static QString prepareLink(const QString& baseFileName, QString link);
    QString prepareDataLink(const qbrunzip *zipData, QString dataFileName, const QStringList& encryptedFiles) const;
    QDomNode processXHTMLNode(qbrunzip *zipData, const QString& xHTMLFileName, const QDomNode& currentNode, const QStringList& encryptedFiles);
    bool processXHTMLFile(QDomNode* xHTMLFileData, qbrunzip* zipData, const QString& xHTMLFileName, const QStringList& encryptedFiles);
    void processRootFileMetadata(const qbrunzip *zipData, const QString& rootFileName, const QDomDocument *rootFileXml, const QMap<QString,QDomElement> *manifestMap, const QStringList& encryptedFiles);
    bool processRootFile(QDomNode* returnValue, qbrunzip* zipData, const QString& rootFileName, const QStringList& encryptedFiles);
    bool parseFile(qbrunzip *zipData);
};

#endif // FORMATEPUB_H
