#ifndef EXPORTTEXT_H
#define EXPORTTEXT_H

#include "export.h"

class ExportText : public Export
{
public:
    ExportText();

    QString getFilter() override;
    void setData(QBRBook *book)  override;
    bool save(QString fileName) override;
private:
    QString htmlData;
};

#endif // EXPORTTEXT_H
