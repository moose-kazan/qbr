#ifndef FILEINFODIALOG_H
#define FILEINFODIALOG_H

#include <QDialog>
#include "format/format.h"

namespace Ui {
class FileInfoDialog;
}

class FileInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileInfoDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~FileInfoDialog();

    void setBookInfo(const qbrbookinfo& newBookInfo);

private:
    qbrbookinfo bookInfo;

    void showEvent(QShowEvent *event);
    Ui::FileInfoDialog *ui;
};

#endif // FILEINFODIALOG_H
