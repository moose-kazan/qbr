#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QCloseEvent>

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadBook(QString fileName);
    void openFile();
    void saveAsFile();
    void helpAbout();
    void helpAboutQt();

    QTextBrowser* browser;

protected:
    void closeEvent(QCloseEvent *event);

private:
    void readSettings();
};
#endif // MAINWINDOW_H
