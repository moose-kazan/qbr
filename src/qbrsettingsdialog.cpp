#include "qbrsettingsdialog.h"
#include "ui_qbrsettingsdialog.h"
#include "qbrcfg.h"

#include <QColorDialog>

QBRSettingsDialog::QBRSettingsDialog(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::QBRSettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle("Settings");
    setModal(true);
    setFixedSize(size());
}

void QBRSettingsDialog::settingsLoad()
{
    QCheckBox *statusBarEnable = findChild<QCheckBox*>("statusBarEnable");
    statusBarEnable->setChecked(qbrcfg::getStatusBarEnabled());

    QCheckBox *enableLoadLast = findChild<QCheckBox*>("enableLoadLast");
    enableLoadLast->setChecked(qbrcfg::getLastOpenedFileEnable());

    switch(qbrcfg::getUiVariant())
    {
        case qbrcfg::uiMenuOnly:
            findChild<QRadioButton*>("uiMenuOnly")->setChecked(true);
        break;

        case qbrcfg::uiToolbarOnly:
            findChild<QRadioButton*>("uiToolbarOnly")->setChecked(true);
        break;

        default:
            findChild<QRadioButton*>("uiMenuAndToolbar")->setChecked(true);
    }

    bookBgColor = qbrcfg::getBookBgColor();
    findChild<QPushButton*>("bgColorButton")->setStyleSheet(QString("background-color: %1").arg(bookBgColor));
}
void QBRSettingsDialog::settingsSave()
{
    qbrcfg::setStatusBarEnabled(findChild<QCheckBox*>("statusBarEnable")->isChecked());
    qbrcfg::setLastOpenedFileEnable(findChild<QCheckBox*>("enableLoadLast")->isChecked());

    if (findChild<QRadioButton*>("uiMenuOnly")->isChecked())
    {
        qbrcfg::setUiVariant(qbrcfg::uiMenuOnly);
    }
    else if (findChild<QRadioButton*>("uiToolbarOnly")->isChecked())
    {
        qbrcfg::setUiVariant(qbrcfg::uiToolbarOnly);
    }
    else
    {
        qbrcfg::setUiVariant(0);
    }
    qbrcfg::setBookBgColor(bookBgColor);
}

void QBRSettingsDialog::bgColorChoose()
{
    QColor color = QColorDialog::getColor(QColor(bookBgColor), this );
    if (color.isValid()) {
        bookBgColor = color.name();
        findChild<QPushButton*>("bgColorButton")->setStyleSheet(QString("background-color: %1").arg(bookBgColor));
    }
}

QBRSettingsDialog::~QBRSettingsDialog()
{
    delete ui;
}

