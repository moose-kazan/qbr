#include "exportmarkdown.h"

#include <QTextDocumentFragment>

ExportMarkdown::ExportMarkdown() {}

QString ExportMarkdown::getFilter() {
    return QString(tr("Markdown documents (%1)")).arg("*.md");
}

QString ExportMarkdown::fromHtml(QString htmlLine) {
    return QTextDocumentFragment::fromHtml(htmlLine).toMarkdown();
}
