#ifndef BOOKSAVER_H
#define BOOKSAVER_H

#include "export/export.h"
#include "export/exporthtml.h"
#include "export/exporttext.h"
#include "export/exportmarkdown.h"

class BookSaver
{
public:
    BookSaver();
    QStringList getFilter();
    Export* exporterByFilter(QString filter);
private:
    QList<Export*> exporters = {
        new ExportHTML(),
        new ExportMarkdown(),
        new ExportText(),
    };
    QStringList filters;
};

#endif // BOOKSAVER_H
