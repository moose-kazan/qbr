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
    bool loadFile(const QString& fileName, const QByteArray& fileData, const qbrunzip* zipData) override;
    QStringList getExtensions() override;
    QString getFormatTitle() override;
    QBRBook* getBook() override;
    bool needUnzip() override;

    const QString EMPTYGIF = "data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7";

private:
    QBRBook* bookInfo;
    static bool isValidFile(const qbrunzip *zipData);
    static QStringList getRootFiles(const qbrunzip *zipData);
    static QStringList getEncryptedFiles(const qbrunzip *zipData);
    static QString expandFileName(const QString& baseFileName, QString expandableFileName);
    static QString prepareLink(const QString& baseFileName, QString link);
    QString prepareDataLink(const qbrunzip *zipData, QString dataFileName, const QStringList& encryptedFiles) const;
    QDomNode processXHTMLNode(const qbrunzip* zipData, const QString& xHTMLFileName, const QDomNode& currentNode, const QStringList& encryptedFiles);
    bool processXHTMLFile(QDomNode* xHTMLFileData, const qbrunzip* zipData, const QString& xHTMLFileName, const QStringList& encryptedFiles);
    void processRootFileMetadata(const qbrunzip *zipData, const QString& rootFileName, const QDomDocument *rootFileXml, const QMap<QString,QDomElement> *manifestMap, const QStringList& encryptedFiles) const;
    bool processRootFile(QDomNode* returnValue, const qbrunzip* zipData, const QString& rootFileName, const QStringList& encryptedFiles);
    bool parseFile(const qbrunzip* zipData);
    void loadToc(const qbrunzip* zipData, const QString& tocFileName, const QString& rootFileName) const;
    static void loadTocItem(const QDomElement& curItem, QList<QBRTocItem>* tocList, const QString& rootFileName);
    void loadTocOld(const qbrunzip *zipData, const QString& tocFileName) const;
    static void loadTocOldItem(const QDomElement& curItem, QList<QBRTocItem>* tocList);
};

#endif // FORMATEPUB_H
