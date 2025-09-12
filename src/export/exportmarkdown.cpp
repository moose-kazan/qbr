#include "exportmarkdown.h"

#include <QTextDocumentFragment>

ExportMarkdown::ExportMarkdown() = default;

QString ExportMarkdown::getFilter() {
    return QString(tr("Markdown documents (%1)")).arg("*.md");
}

QString ExportMarkdown::fromBook(QBRBook *book) {
    QStringList markdownLinesSrc = QTextDocumentFragment::fromHtml(book->html).toMarkdown(QTextDocument::MarkdownNoHTML).split("\n");
    QStringList markdownDocument;

    for (int i = 0; i < markdownLinesSrc.count(); i++) {
        if (markdownLinesSrc.at(i).startsWith("![image](data:")) {
            markdownDocument.append("[IMG]");
            continue;
        }
        markdownDocument.append(markdownLinesSrc.at(i));
    }

    return markdownDocument.join("\n");
}
