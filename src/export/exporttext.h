#ifndef EXPORTTEXT_H
#define EXPORTTEXT_H

#include "export.h"

class ExportText : public Export
{
public:
    ExportText();

    QString getFilter() override;
    QString fromHtml(QString htmlLine) override;
};

#endif // EXPORTTEXT_H
