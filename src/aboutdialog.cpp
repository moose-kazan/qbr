#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "version.h"

#include <QLabel>
#include <QPushButton>

AboutDialog::AboutDialog(QWidget* parent, const Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("About"));
    setModal(true);
    setFixedSize(size());

    QString versionString = QString(tr("Version: %1")).arg(PROJECT_VERSION_STRING);
    QLabel* versionLabel = findChild<QLabel*>("appVersion");
    versionLabel->setText(versionString);

    const auto* okButton = findChild<QPushButton*>("okButton");
    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}
