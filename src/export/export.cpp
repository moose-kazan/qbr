#include "export.h"

Export::Export() {}

QString Export::getFilter() {
    return "";
}

QString Export::fromHtml(const QString htmlLine) {
    return htmlLine;
}
