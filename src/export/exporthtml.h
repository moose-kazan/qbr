#ifndef EXPORTHTML_H
#define EXPORTHTML_H

#include "export.h"

class ExportHTML : public Export
{
public:
    ExportHTML();

    QString getFilter() override;
    QString fromBook(QBRBook *book)  override;
};

#endif // EXPORTHTML_H
