#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~FindDialog();

private:
    void showEvent(QShowEvent *event);
    Ui::FindDialog *ui;
};

#endif // FINDDIALOG_H
