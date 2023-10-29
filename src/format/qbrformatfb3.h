#ifndef QBRFORMATFB3_H
#define QBRFORMATFB3_H

#include "../qbrtemplate.h"
#include "../qbrformat.h"
#include "../libs/qbrzip.h"
#include <QString>
#include <QByteArray>
#include <QDomNode>
#include <QHash>


class QBRFormatFB3 : public QBRFormat
{
public:
    QBRFormatFB3();
    bool loadFile(QString fileName, QByteArray fileData) override;
    QStringList getExtensions() override;
    QString getHtml() override;

private:
    QString htmlData;
    bool parseFile(QByteArray fileData);
    QString parseFB3Node(QDomNode xmlNode);
    QString parseFB3TextFromNode(QDomNode xmlNode);
    qbrzip unZip;
    QHash<QString, QString> fb3_binaries;
};

#endif // QBRFORMATFB3_H
