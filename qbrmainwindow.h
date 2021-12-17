#ifndef QBRMAINWINDOW_H
#define QBRMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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

private:
    Ui::QBRMainWindow *ui;
};

#endif // QBRMAINWINDOW_H
