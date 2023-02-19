#include "qbrformat.h"

QBRFormat::QBRFormat()
{

}

bool QBRFormat::isZipFile(QByteArray data)
{
    char zipSignature[] = {0x50, 0x4B, 0x03, 0x04};
    return data.startsWith(zipSignature);
}
