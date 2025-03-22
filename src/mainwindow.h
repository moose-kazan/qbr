#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>

#include "format/format.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "fileinfodialog.h"
#include "settingsdialog.h"
#include "bookloader.h"
#include "booksaver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    void readSettings();
    void loadBook(QString fileName);
    void bookLoadFinished(bool ok);
    ~MainWindow();
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

    AboutDialog *aboutDlg;
    FindDialog *findDlg;
    FileInfoDialog *fileInfoDlg;
    SettingsDialog *settingsDlg;
    QFileDialog *openFileDlg;

    Ui::MainWindow *ui;

    BookLoader *bookLoader;
    BookSaver *bookSaver;

    QBRBookInfo bookInfo;
};

#endif // MAINWINDOW_H
