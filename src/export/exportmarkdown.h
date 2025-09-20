#ifndef EXPORTMARKDOWN_H
#define EXPORTMARKDOWN_H

#include "export.h"

class ExportMarkdown final : public Export
{
public:
    ExportMarkdown();

    QString getFilter() override;
    void setData(QBRBook *book)  override;
    bool save(QString fileName) override;
private:
    QString htmlData;
};

#endif // EXPORTMARKDOWN_H
