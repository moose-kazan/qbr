#ifndef QBRFINDDIALOG_H
#define QBRFINDDIALOG_H

#include <QDialog>

namespace Ui {
class QBRFindDialog;
}

class QBRFindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QBRFindDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QBRFindDialog();

private:
    void showEvent(QShowEvent *event);
    Ui::QBRFindDialog *ui;
};

#endif // QBRFINDDIALOG_H
