#include "exporttext.h"

#include <QTextDocumentFragment>

ExportText::ExportText() = default;

QString ExportText::getFilter() {
    return QString(tr("Plain text files (%1)")).arg("*.txt");
}

void ExportText::setData(QBRBook *book) {
    htmlData = QTextDocumentFragment::fromHtml(book->html).toPlainText();
}
bool ExportText::save(const QString fileName)
{
    return _save(fileName, htmlData.toUtf8());
}
