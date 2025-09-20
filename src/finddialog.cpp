#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent, const Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::FindDialog) {

  ui->setupUi(this);
  setWindowTitle(tr("Find Text"));
  setModal(true);
  setFixedSize(size());
}

void FindDialog::showEvent(QShowEvent *event) {
  (void)event;
  findChild<QLineEdit *>("findText")->setFocus();
}

FindDialog::~FindDialog() { delete ui; }
