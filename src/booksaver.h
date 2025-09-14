#ifndef BOOKSAVER_H
#define BOOKSAVER_H

#include "export/export.h"
#include "export/exporthtml.h"
#include "export/exporttext.h"
#include "export/exportepub.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include "export/exportmarkdown.h"
#endif

class BookSaver
{
public:
    BookSaver();
    QStringList getFilter();
    Export* exporterByFilter(const QString& filter) const;
private:
    QList<Export*> exporters = {
        new ExportHTML(),
        new ExportText(),
        new ExportEPub(),
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
        new ExportMarkdown(),
#endif
    };
    QStringList filters;
};

#endif // BOOKSAVER_H
