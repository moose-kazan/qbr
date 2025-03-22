#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

AboutDialog::AboutDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::AboutDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("About"));
  setModal(true);
  setFixedSize(size());

  QPushButton *okButton = findChild<QPushButton *>("okButton");
  connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}
