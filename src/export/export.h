#ifndef EXPORT_H
#define EXPORT_H

#include "../libs/qbrbookinfo.h"

#include <QObject>
#include <QString>

class Export : QObject
{
    Q_OBJECT
public:
    Export();

    virtual QString getFilter();
    virtual QString fromBook(QBRBook *book);
};

#endif // EXPORT_H
