#ifndef QBRFILEINFODIALOG_H
#define QBRFILEINFODIALOG_H

#include <QDialog>
#include "format/qbrformat.h"

namespace Ui {
class QBRFileInfoDialog;
}

class QBRFileInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QBRFileInfoDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QBRFileInfoDialog();

    void setBookInfo(QBRBookInfo newBookInfo);

private:
    QBRBookInfo bookInfo;

    void showEvent(QShowEvent *event);
    Ui::QBRFileInfoDialog *ui;
};

#endif // QBRFILEINFODIALOG_H
