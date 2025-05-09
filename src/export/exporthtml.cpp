#include "exporthtml.h"

ExportHTML::ExportHTML() {}

QString ExportHTML::getFilter() {
    return QString(tr("HTML pages (%1)")).arg("*.htm *.html");
}

QString ExportHTML::fromHtml(const QString htmlLine) {
    return htmlLine;
}
