#ifndef QBRABOUTDIALOG_H
#define QBRABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class QBRAboutDialog;
}

class QBRAboutDialog : public QDialog
{
    Q_OBJECT
public:
    QBRAboutDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
private:
    Ui::QBRAboutDialog *ui;
};

#endif // QBRABOUTDIALOG_H
