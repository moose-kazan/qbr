#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
