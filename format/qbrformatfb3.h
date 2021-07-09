#ifndef QBRFORMATFB3_H
#define QBRFORMATFB3_H

#include "../qbrtemplate.h"
#include "../qbrformat.h"
#include "../qbrzip.h"
#include <QString>
#include <QByteArray>
#include <QDomNode>
#include <QHash>


class qbrformatfb3 : public qbrformat
{
public:
    qbrformatfb3();
    bool loadFile(QString fileName, QByteArray fileData) override;
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
