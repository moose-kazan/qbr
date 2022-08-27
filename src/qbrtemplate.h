#ifndef QBRTEMPLATE_H
#define QBRTEMPLATE_H

#include <QString>


class qbrtemplate
{
public:
    qbrtemplate();
    static QString header();
    static QString footer();
    static QString empty();
    static QString emptyAsDataUri();
};

#endif // QBRTEMPLATE_H
