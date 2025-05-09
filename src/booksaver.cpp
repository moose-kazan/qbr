#include "booksaver.h"

BookSaver::BookSaver() {
    for (int i = 0; i < exporters.count(); i++) {
        filters.append(exporters.at(i)->getFilter());
    }
}

QStringList BookSaver::getFilter() {
    return filters;
}

Export* BookSaver::exporterByFilter(const QString& filter) const
{
    int idx = filters.indexOf(filter);
    return exporters.at(idx);
}
