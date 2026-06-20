#include "booksaver.h"

BookSaver::BookSaver() {
    exporters.append(new ExportHTML());
    exporters.append(new ExportText());
    exporters.append(new ExportEPub());
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
    exporters.append(new ExportMarkdown());
#endif

    for (int i = 0; i < exporters.count(); i++) {
        filters.append(exporters.at(i)->getFilter());
    }
}

BookSaver::~BookSaver()
{
    for (int i = 0; i < exporters.count(); i++)
    {
        delete exporters.at(i);
    }
}

QStringList BookSaver::getFilter() {
    return filters;
}

Export* BookSaver::exporterByFilter(const QString& filter) const
{
    const long idx = filters.indexOf(filter);
    if (idx < 0)
    {
        qWarning() << Q_FUNC_INFO;
        return nullptr;
    }
    return exporters.at(idx);
}
