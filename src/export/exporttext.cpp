#include "exporttext.h"

#include <QTextDocumentFragment>

ExportText::ExportText() = default;

QString ExportText::getFilter() {
    return QString(tr("Plain text files (%1)")).arg("*.txt");
}

QString ExportText::fromBook(QBRBook *book) {
    return QTextDocumentFragment::fromHtml(book->html).toPlainText();
}
