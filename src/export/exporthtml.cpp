#include "exporthtml.h"

ExportHTML::ExportHTML() = default;

QString ExportHTML::getFilter() {
    return QString(tr("HTML pages (%1)")).arg("*.htm *.html");
}

void ExportHTML::setData(QBRBook *book) {
    htmlData = book->html;
}

bool ExportHTML::save(const QString fileName)
{
    return _save(fileName, htmlData.toUtf8());
}
