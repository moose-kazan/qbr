//
// Created by moose on 13.09.25.
//

#include "qbrbookinfo.h"

QBRTocItem::QBRTocItem() = default;
QBRTocItem::QBRTocItem(const QString& Title, const QString& Anchor)
{
    this->Title = Title;
    this->Anchor = Anchor;
}


QBRBookMetadata::QBRBookMetadata()= default;

void QBRBookMetadata::clear()
{
    Author = "";
    Title = "";
    Description = "";
    FileFormat = "";
    Cover = QImage();
    Toc.clear();
}

void QBRBookMetadata::copy(const QBRBookMetadata* other)
{
    Author = other->Author;
    Title = other->Title;
    Description = other->Description;
    FileFormat = other->FileFormat;
    Cover = other->Cover;
    Toc = other->Toc;
}

QBRBook::QBRBook()
{
    metadata = new QBRBookMetadata();
};

void QBRBook::clear()
{
    metadata->clear();
    html = "";
}
void QBRBook::copy(const QBRBook* other)
{
    metadata->copy(other->metadata);
    html = other->html;
}
