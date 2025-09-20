#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QLabel>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget *parent, const Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("About"));
  setModal(true);
  setFixedSize(size());

  const auto *okButton = findChild<QPushButton *>("okButton");
  connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}
