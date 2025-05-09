#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SettingsDialog();
    void settingsLoad();
    void settingsSave();

private:
    Ui::SettingsDialog *ui;
    QString bookBgColor;

private slots:
    void bgColorChoose();
    void customCssSelect();
    void customCssEnable(int state) const;
};

#endif // SETTINGSDIALOG_H
