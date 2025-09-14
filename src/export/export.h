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
    virtual void setData(QBRBook *book);
    virtual bool save(QString fileName);
protected:
    static bool _save(const QString& fileName, const QByteArray& data);
};

#endif // EXPORT_H
