#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
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
    void naviGoBack();
    void naviGoForward();

    QWebEngineView* browser;

protected:
    void closeEvent(QCloseEvent *event);
    void bookLoadFinished(bool ok);

private:
    void readSettings();
};
#endif // MAINWINDOW_H
