#include "exporthtml.h"

ExportHTML::ExportHTML() = default;

QString ExportHTML::getFilter() {
    return QString(tr("HTML pages (%1)")).arg("*.htm *.html");
}

QString ExportHTML::fromBook(QBRBook *book) {
    return book->html;
}
