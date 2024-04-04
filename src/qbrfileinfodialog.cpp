#include "qbrfileinfodialog.h"
#include "ui_qbrfileinfodialog.h"


QBRFileInfoDialog::QBRFileInfoDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::QBRFileInfoDialog) {

    ui->setupUi(this);
    setWindowTitle(tr("Book Info"));
    setModal(true);
    setFixedSize(size());

}

void QBRFileInfoDialog::setBookInfo(QBRBookInfo newBookInfo)
{
    bookInfo = newBookInfo;
}

void QBRFileInfoDialog::showEvent(QShowEvent *event)
{
    (void)event;
    QTableWidget *tw = findChild<QTableWidget *>("bookInfo");

    tw->clear();

    tw->setColumnCount(1);
    tw->horizontalHeader()->hide();

    tw->insertRow(0);
    tw->setVerticalHeaderItem(0, new QTableWidgetItem(tr("File type")));
    tw->setItem(0, 0, new QTableWidgetItem(bookInfo.FileFormat));

    tw->insertRow(1);
    tw->setVerticalHeaderItem(1, new QTableWidgetItem(tr("Book author")));
    tw->setItem(1, 0, new QTableWidgetItem(bookInfo.Author));

    tw->insertRow(2);
    tw->setVerticalHeaderItem(2, new QTableWidgetItem(tr("Book title")));
    tw->setItem(2, 0, new QTableWidgetItem(bookInfo.Title));

    tw->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

    for (int i = 0; i < 3; i++) {
        tw->item(i, 0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
}

QBRFileInfoDialog::~QBRFileInfoDialog()
{
    delete ui;
}
