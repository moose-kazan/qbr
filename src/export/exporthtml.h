#ifndef EXPORTHTML_H
#define EXPORTHTML_H

#include "export.h"

class ExportHTML : public Export
{
public:
    ExportHTML();

    QString getFilter() override;
    QString fromHtml(QString htmlLine) override;
};

#endif // EXPORTHTML_H
