#include "qbrmainwindow.h"
#include "qbrcfg.h"

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
    else if (qbrcfg::getLastOpenedFileEnable() && qbrcfg::getLastOpenedFile().length() > 0) {
        w.loadBook(qbrcfg::getLastOpenedFile());
    }
    return a.exec();
}
