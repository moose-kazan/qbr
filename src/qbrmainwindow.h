#ifndef QBRMAINWINDOW_H
#define QBRMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "format/qbrformat.h"

namespace Ui {
class QBRMainWindow;
}

class QBRMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QBRMainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    void readSettings();
    void loadBook(QString fileName);
    void bookLoadFinished(bool ok);
    ~QBRMainWindow();
    void setCurrentFileName(QString fileName);
    QString getCurrentFileName();

    QLabel* statusBarFileName;

private slots:
    void openFile();
    void saveFileAs();
    void helpAbout();
    void helpAboutQt();
    void naviGoBack();
    void naviGoForward();
    void naviFindGo();
    void naviFind();
    void settingsShow();

private:
    void readState();
    void readBookSettings();
    void positionSave();
    void positionRestore();
    QString currentFileName;

    Ui::QBRMainWindow *ui;
    QList<QBRFormat*> bookParsers;
};

#endif // QBRMAINWINDOW_H
