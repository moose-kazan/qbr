#include "exporttext.h"

#include <QTextDocumentFragment>

ExportText::ExportText() {}

QString ExportText::getFilter() {
    return QString(tr("Plain text files (%1)")).arg("*.txt");
}

QString ExportText::fromHtml(QString htmlLine) {
    return QTextDocumentFragment::fromHtml(htmlLine).toPlainText();
}
