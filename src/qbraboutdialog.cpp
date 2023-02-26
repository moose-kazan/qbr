#include "qbraboutdialog.h"
#include "ui_qbraboutdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

QBRAboutDialog::QBRAboutDialog(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::QBRAboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("About"));
    setModal(true);
    setFixedSize(size());

    QPushButton *okButton = findChild<QPushButton*>("okButton");
    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}
