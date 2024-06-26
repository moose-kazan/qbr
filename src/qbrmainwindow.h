#ifndef QBRMAINWINDOW_H
#define QBRMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>

#include "format/qbrformat.h"
#include "qbraboutdialog.h"
#include "qbrfinddialog.h"
#include "qbrfileinfodialog.h"
#include "qbrsettingsdialog.h"

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
    void naviFind();
    void settingsShow();
    void fileBookInfo();

private:
    void readState();
    void readBookSettings();
    void positionSave();
    void positionRestore();
    QString currentFileName;

    QBRAboutDialog *aboutDlg;
    QBRFindDialog *findDlg;
    QBRFileInfoDialog *fileInfoDlg;
    QBRSettingsDialog *settingsDlg;
    QFileDialog *openFileDlg;

    Ui::QBRMainWindow *ui;
    QList<QBRFormat*> bookParsers;

    QBRBookInfo bookInfo;
};

#endif // QBRMAINWINDOW_H
