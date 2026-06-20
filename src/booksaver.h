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
    ~BookSaver();
    QStringList getFilter();
    [[nodiscard]] Export* exporterByFilter(const QString& filter) const;
private:
    QList<Export*> exporters = {};
    QStringList filters;
};

#endif // BOOKSAVER_H
