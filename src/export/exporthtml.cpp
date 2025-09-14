#include "exporthtml.h"

ExportHTML::ExportHTML() = default;

QString ExportHTML::getFilter() {
    return QString(tr("HTML pages (%1)")).arg("*.htm *.html");
}

void ExportHTML::setData(QBRBook *book) {
    htmlData = book->html;
}

bool ExportHTML::save(QString filename)
{
    return _save(filename, htmlData.toUtf8());
}
