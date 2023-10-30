#include "qbraboutdialog.h"
#include "ui_qbraboutdialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

QBRAboutDialog::QBRAboutDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::QBRAboutDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("About"));
  setModal(true);
  setFixedSize(size());

  QPushButton *okButton = findChild<QPushButton *>("okButton");
  connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}
