#ifndef EXPORTMARKDOWN_H
#define EXPORTMARKDOWN_H

#include "export.h"

class ExportMarkdown : public Export
{
public:
    ExportMarkdown();

    QString getFilter() override;
    QString fromBook(QBRBook *book)  override;
};

#endif // EXPORTMARKDOWN_H
