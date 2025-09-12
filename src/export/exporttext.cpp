#include "exporttext.h"

#include <QTextDocumentFragment>

ExportText::ExportText() = default;

QString ExportText::getFilter() {
    return QString(tr("Plain text files (%1)")).arg("*.txt");
}

QString ExportText::fromHtml(const QString htmlLine) {
    return QTextDocumentFragment::fromHtml(htmlLine).toPlainText();
}
