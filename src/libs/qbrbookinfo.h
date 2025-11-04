//
// Created by moose on 13.09.25.
//

#ifndef QBRBOOKINFO_H
#define QBRBOOKINFO_H

#include <QStandardItemModel>

class QBRTocItem
{
public:
    QBRTocItem();
    QBRTocItem(const QString& Title, const QString& Anchor);
    QString Title;
    QString Anchor;
    QList<QBRTocItem> Childs;
};

class QBRBookMetadata
{
public:
    QBRBookMetadata();
    QString Author;
    QString Title;
    QString Description;
    QString FileFormat;
    QImage Cover;
    QList<QBRTocItem> Toc;
    void clear();
    void copy(const QBRBookMetadata* other);
};

class QBRBook
{
    public:
    QBRBook();
    QBRBookMetadata* metadata;
    QString html;
    void clear();
    void copy(const QBRBook* other);
};




#endif //QBRBOOKINFO_H
