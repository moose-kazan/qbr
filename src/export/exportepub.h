#ifndef EXPORTEPUB_H
#define EXPORTEPUB_H

#include "export.h"
#include "../libs/qbrzip.h"

class ExportEPub : public Export{
public:
    ExportEPub();
    QString getFilter() override;
    void setData(QBRBook *book)  override;
    bool save(QString fileName) override;
private:
    static QByteArray prepareContainerXml();
    QByteArray prepareContentOpf();
    static QByteArray prepareNavXhtml();
    QBRBook *book;
    qbrzip *zipWriter;
};



#endif //EXPORTEPUB_H
