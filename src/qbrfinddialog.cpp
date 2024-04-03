#include "qbrfinddialog.h"
#include "ui_qbrfinddialog.h"


QBRFindDialog::QBRFindDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::QBRFindDialog) {

    ui->setupUi(this);
    setWindowTitle(tr("Find Text"));
    setModal(true);
    setFixedSize(size());

}

void QBRFindDialog::showEvent(QShowEvent *event)
{
    (void)event;
    findChild<QLineEdit *>("findText")->setFocus();
}

QBRFindDialog::~QBRFindDialog()
{
    delete ui;
}
