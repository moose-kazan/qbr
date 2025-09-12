#include "export.h"

Export::Export() {}

QString Export::getFilter() {
    return "";
}

QString Export::fromBook(QBRBook *book) {
    return book->html;
}
