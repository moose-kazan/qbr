#ifndef HELPERS_H
#define HELPERS_H

#include <QString>


class Helpers
{
public:
    Helpers();
    static qreal getDesktopScale();
    static int getDesktopScaledInt(int n);
    static QString getDesktopScaledIntAsQString(int n);
};

#endif // HELPERS_H
