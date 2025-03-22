#ifndef EXPORT_H
#define EXPORT_H

#include <QObject>
#include <QString>

class Export : QObject
{
    Q_OBJECT
public:
    Export();

    virtual QString getFilter();
    virtual QString fromHtml(QString htmlLine);
};

#endif // EXPORT_H
