//
// Created by moose on 13.09.25.
//

#ifndef QBRBOOKINFO_H
#define QBRBOOKINFO_H

#include <QImage>


struct qbrbookinfo {
    QString Author;
    QString Title;
    QString Description;
    QString FileFormat;
    QImage Cover;
    void clear() {
        Author = "";
        Title = "";
        Description = "";
        FileFormat = "";
        Cover = QImage();
    }
};

struct QBRBook {
    qbrbookinfo metadata;
    QString html;
};



#endif //QBRBOOKINFO_H
