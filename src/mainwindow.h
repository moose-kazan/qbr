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

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    void readSettings() const;
    void loadBook(const QString& fileName);
    void bookLoadFinished(bool ok);
    ~MainWindow() override;
    void setCurrentFileName(const QString& fileName);
    QString getCurrentFileName();

    QLabel* statusBarFileName;

private slots:
    void openFile();
    void saveFileAs();
    void helpAbout() const;
    void helpAboutQt();
    void naviGoBack() const;
    void naviGoForward() const;
    void naviFind();
    void settingsShow() const;
    void fileBookInfo();

private:
    void readState();
    void readBookSettings() const;
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

    QBRBook bookInfo;
};

#endif // MAINWINDOW_H
