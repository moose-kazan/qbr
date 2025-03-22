#ifndef EXPORTMARKDOWN_H
#define EXPORTMARKDOWN_H

#include "export.h"

class ExportMarkdown : public Export
{
public:
    ExportMarkdown();

    QString getFilter() override;
    QString fromHtml(QString htmlLine) override;
};

#endif // EXPORTMARKDOWN_H
