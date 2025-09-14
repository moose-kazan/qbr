#ifndef EXPORTHTML_H
#define EXPORTHTML_H

#include "export.h"

class ExportHTML : public Export
{
public:
    ExportHTML();

    QString getFilter() override;
    void setData(QBRBook *book)  override;
    bool save(QString fileName) override;
private:
    QString htmlData;
};

#endif // EXPORTHTML_H
