#include "exporthtml.h"

ExportHTML::ExportHTML() = default;

QString ExportHTML::getFilter() {
    return QString(tr("HTML pages (%1)")).arg("*.htm *.html");
}

QString ExportHTML::fromHtml(const QString htmlLine) {
    return htmlLine;
}
