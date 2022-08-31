#ifndef QBRSETTINGSDIALOG_H
#define QBRSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class QBRSettingsDialog;
}

class QBRSettingsDialog : public QDialog
{
    //Q_OBJECT
public:
    QBRSettingsDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QBRSettingsDialog();
    void settingsLoad();
    void settingsSave();

private:
    Ui::QBRSettingsDialog *ui;
};

#endif // QBRSETTINGSDIALOG_H
