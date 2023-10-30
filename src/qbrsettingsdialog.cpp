#include "qbrsettingsdialog.h"
#include "ui_qbrsettingsdialog.h"
#include "qbrcfg.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QUrl>
#include <QDebug>

QBRSettingsDialog::QBRSettingsDialog(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::QBRSettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Settings"));
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

    findChild<QCheckBox*>("customCssEnable")->setChecked(qbrcfg::getCustomStyleEnabled());
    findChild<QLineEdit*>("customCssUrl")->setEnabled(qbrcfg::getCustomStyleEnabled());
    findChild<QLineEdit*>("customCssUrl")->setText(qbrcfg::getCustomStyleUrl());
    findChild<QPushButton*>("customCssSelect")->setEnabled(qbrcfg::getCustomStyleEnabled());
}
void QBRSettingsDialog::settingsSave()
{
    qbrcfg::setStatusBarEnabled(findChild<QCheckBox*>("statusBarEnable")->isChecked());
    qbrcfg::setLastOpenedFileEnable(findChild<QCheckBox*>("enableLoadLast")->isChecked());

    qbrcfg::setCustomStaticEnabled(findChild<QCheckBox*>("customCssEnable")->isChecked());
    qbrcfg::setCustomStyleUrl(findChild<QLineEdit*>("customCssUrl")->text());

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

void QBRSettingsDialog::customCssSelect()
{
    QString fileName = QFileDialog::getOpenFileName(
                           this,
                           tr("Select File"),
                           QDir::homePath(),
                           tr("Stylesheets (%1)").arg("*.css")
                       );
    if (fileName != "")
    {
        findChild<QLineEdit*>("customCssUrl")->setText(QUrl::fromLocalFile(fileName).toString());
    }
}

void QBRSettingsDialog::customCssEnable(int state)
{
    if (state == Qt::Checked)
    {
        findChild<QLineEdit*>("customCssUrl")->setEnabled(true);
        findChild<QPushButton*>("customCssSelect")->setEnabled(true);
    }
    else if (state == Qt::Unchecked)
    {
        findChild<QLineEdit*>("customCssUrl")->setEnabled(false);
        findChild<QPushButton*>("customCssSelect")->setEnabled(false);
    }

}

QBRSettingsDialog::~QBRSettingsDialog()
{
    delete ui;
}

