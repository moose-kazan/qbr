#ifndef QBRSETTINGSDIALOG_H
#define QBRSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class QBRSettingsDialog;
}

class QBRSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    QBRSettingsDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QBRSettingsDialog();
    void settingsLoad();
    void settingsSave();

private:
    Ui::QBRSettingsDialog *ui;
    QString bookBgColor;

private slots:
    void bgColorChoose();
    void customCssSelect();
    void customCssEnable(int state);
};

#endif // QBRSETTINGSDIALOG_H
