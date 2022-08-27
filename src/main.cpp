#include "qbrmainwindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/64x64/qbr.png"));
    QBRMainWindow w;
    w.show();
    if (argc > 1)
    {
        w.loadBook(argv[1]);
    }
    return a.exec();
}
