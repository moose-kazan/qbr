#ifndef QBRFORMATCBZ_H
#define QBRFORMATCBZ_H

#include "../qbrformat.h"
#include <QByteArray>
#include <QString>


class qbrformatcbz : public qbrformat
{
public:
    qbrformatcbz();
    bool loadFile(QString fileName, QByteArray fileData) override;
    QString getHtml() override;
private:
    QString htmlData;
};

#endif // QBRFORMATCBZ_H
