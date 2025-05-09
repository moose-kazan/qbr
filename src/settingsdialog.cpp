#include "settingsdialog.h"
#include "libs/settings.h"
#include "ui_settingsdialog.h"

#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QUrl>

SettingsDialog::SettingsDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("Settings"));
  setModal(true);
  setFixedSize(size());
}

void SettingsDialog::settingsLoad() {
  QCheckBox *statusBarEnable = findChild<QCheckBox *>("statusBarEnable");
  statusBarEnable->setChecked(Settings::getStatusBarEnabled());

  QCheckBox *enableLoadLast = findChild<QCheckBox *>("enableLoadLast");
  enableLoadLast->setChecked(Settings::getLastOpenedFileEnable());

  switch (Settings::getUiVariant()) {
  case Settings::uiMenuOnly:
    findChild<QRadioButton *>("uiMenuOnly")->setChecked(true);
    break;

  case Settings::uiToolbarOnly:
    findChild<QRadioButton *>("uiToolbarOnly")->setChecked(true);
    break;

  default:
    findChild<QRadioButton *>("uiMenuAndToolbar")->setChecked(true);
  }

  bookBgColor = Settings::getBookBgColor();
  findChild<QPushButton *>("bgColorButton")
      ->setStyleSheet(QString("background-color: %1").arg(bookBgColor));

  findChild<QCheckBox *>("customCssEnable")
      ->setChecked(Settings::getCustomStyleEnabled());
  findChild<QLineEdit *>("customCssUrl")
      ->setEnabled(Settings::getCustomStyleEnabled());
  findChild<QLineEdit *>("customCssUrl")->setText(Settings::getCustomStyleUrl());
  findChild<QPushButton *>("customCssSelect")
      ->setEnabled(Settings::getCustomStyleEnabled());
}
void SettingsDialog::settingsSave() {
  Settings::setStatusBarEnabled(
      findChild<QCheckBox *>("statusBarEnable")->isChecked());
  Settings::setLastOpenedFileEnable(
      findChild<QCheckBox *>("enableLoadLast")->isChecked());

  Settings::setCustomStaticEnabled(
      findChild<QCheckBox *>("customCssEnable")->isChecked());
  Settings::setCustomStyleUrl(findChild<QLineEdit *>("customCssUrl")->text());

  if (findChild<QRadioButton *>("uiMenuOnly")->isChecked()) {
    Settings::setUiVariant(Settings::uiMenuOnly);
  } else if (findChild<QRadioButton *>("uiToolbarOnly")->isChecked()) {
    Settings::setUiVariant(Settings::uiToolbarOnly);
  } else {
    Settings::setUiVariant(0);
  }
  Settings::setBookBgColor(bookBgColor);
}

void SettingsDialog::bgColorChoose() {
  QColor color = QColorDialog::getColor(QColor(bookBgColor), this);
  if (color.isValid()) {
    bookBgColor = color.name();
    findChild<QPushButton *>("bgColorButton")
        ->setStyleSheet(QString("background-color: %1").arg(bookBgColor));
  }
}

void SettingsDialog::customCssSelect() {
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath(),
                                   tr("Stylesheets (%1)").arg("*.css"));
  if (fileName != "") {
    findChild<QLineEdit *>("customCssUrl")
        ->setText(QUrl::fromLocalFile(fileName).toString());
  }
}

void SettingsDialog::customCssEnable(int state) const
{
  if (state == Qt::Checked) {
    findChild<QLineEdit *>("customCssUrl")->setEnabled(true);
    findChild<QPushButton *>("customCssSelect")->setEnabled(true);
  } else if (state == Qt::Unchecked) {
    findChild<QLineEdit *>("customCssUrl")->setEnabled(false);
    findChild<QPushButton *>("customCssSelect")->setEnabled(false);
  }
}

SettingsDialog::~SettingsDialog() { delete ui; }
