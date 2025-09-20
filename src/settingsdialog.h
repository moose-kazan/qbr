#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SettingsDialog() override;
    void settingsLoad();
    void settingsSave() const;

private:
    Ui::SettingsDialog *ui;
    QString bookBgColor;

private slots:
    void bgColorChoose();
    void customCssSelect();
    void customCssEnable(int state) const;
};

#endif // SETTINGSDIALOG_H
