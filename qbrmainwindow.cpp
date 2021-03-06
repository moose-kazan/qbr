#include "qbrmainwindow.h"
#include "ui_qbrmainwindow.h"
#include "aboutdialog.h"
#include "qbrtemplate.h"
#include "qbrwebenginepage.h"
#include "qbrcfg.h"

#include "qbrformat.h"
#include "format/qbrformatamb.h"
#include "format/qbrformatcbz.h"
#include "format/qbrformatfb2.h"
#include "format/qbrformatfb3.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QTemporaryFile>
#include <QWebEngineHistory>

QBRMainWindow::QBRMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QBRMainWindow)
{
    ui->setupUi(this);

    findChild<QWidget*>("findWidget")->setVisible(false);

    QWebEngineView *browser = findChild<QWebEngineView*>("browser");
    browser->setPage(new qbrWebEnginePage);
    browser->setContextMenuPolicy(Qt::NoContextMenu);
    browser->load(QUrl(qbrtemplate::emptyAsDataUri()));
    connect(findChild<QWebEngineView*>("browser"), &QWebEngineView::loadFinished, this, &QBRMainWindow::bookLoadFinished);

    statusBarFileName = new QLabel(this);
    statusBar()->addWidget(statusBarFileName);

    setWindowTitle(tr("Qt Book Reader"));
    setGeometry(300, 300, 480, 320);
    readSettings();

}

void QBRMainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), tr("Books (*.amb *.fb2 *.fb3 *.cbz)"));
    if (fileName != "")
    {
        loadBook(fileName);
    }
}

void QBRMainWindow::saveFileAs()
{
    // If file no loaded
    if (statusBarFileName->text().length() < 1)
    {
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), tr(""), tr("Html pages (*.html *.html)"));
    if (fileName != "")
    {
        findChild<QWebEngineView*>("browser")->page()->toHtml([fileName](QString htmlData){
            QFile f(fileName);
            if(f.open(QIODevice::WriteOnly))
            {
                f.write(htmlData.toUtf8());
                f.close();
            }
        });
        //browser->toHtml();
    }

}

void QBRMainWindow::helpAbout()
{
    AboutDialog *aboutDlg = new AboutDialog(this, Qt::Dialog);
    aboutDlg->show();
}

void QBRMainWindow::helpAboutQt()
{
    QMessageBox::aboutQt(this, tr("Qt Book Reader"));
}

void QBRMainWindow::naviGoBack()
{
    findChild<QWebEngineView*>("browser")->back();
}

void QBRMainWindow::naviGoForward()
{
    findChild<QWebEngineView*>("browser")->forward();
}

void QBRMainWindow::naviFindGo()
{
    findChild<QWebEngineView*>("browser")->findText(
            findChild<QLineEdit*>("findText")->text()
        );
}

void QBRMainWindow::naviFind()
{
    if (statusBarFileName->text().length() < 1)
    {
        return;
    }
    bool visible = findChild<QWidget*>("findWidget")->isVisible();
    if (visible)
    {
        findChild<QWebEngineView*>("browser")->findText("");
        findChild<QLineEdit*>("findText")->setText("");
        findChild<QWidget*>("findWidget")->hide();
    }
    else
    {
        findChild<QWidget*>("findWidget")->show();
        findChild<QLineEdit*>("findText")->setFocus();
    }
}

void QBRMainWindow::closeEvent(QCloseEvent *event)
{
    positionSave();

    QSettings* cfg = qbrcfg::getInstance();
    cfg->setValue("geometry", saveGeometry());
    cfg->setValue("windowState", saveState());
    QMainWindow::closeEvent(event);

}

void QBRMainWindow::readSettings()
{
    QSettings* cfg = qbrcfg::getInstance();
    restoreGeometry(cfg->value("geometry").toByteArray());
    restoreState(cfg->value("windowState").toByteArray());
}

void QBRMainWindow::loadBook(QString fileName)
{
    positionSave();

    QByteArray fileData;
    try
    {
        QFile f(fileName);
        f.open(QIODevice::ReadOnly);
        QDataStream in(&f);
        int buff_size = 4096;
        char buff[buff_size];
        while (!in.atEnd())
        {
            int readed = in.readRawData(buff, buff_size);
            if (readed > 0)
            {
                fileData.append(buff, readed);
            }
        }
        f.close();
    }
    catch (...)
    {
        QMessageBox::critical(this, tr("Can't load book"), tr("I/O error!"));
        return;
    }
    QList<qbrformat*> parsers;
    parsers.append(new qbrformatamb());
    parsers.append(new qbrformatcbz());
    parsers.append(new qbrformatfb2());
    parsers.append(new qbrformatfb3());

    for (int i = 0; i < parsers.count(); i++) {
        if (parsers.at(i)->loadFile(fileName, fileData)) {
            QTemporaryFile f(QDir::tempPath() + "/QBR.XXXXXXX.html");
            f.setAutoRemove(false);
            if(f.open())
            {
                f.write(parsers.at(i)->getHtml().toUtf8());
                f.close();

                findChild<QWebEngineView*>("browser")->load(QUrl::fromLocalFile(f.fileName()));
                statusBarFileName->setText(fileName);
            }
            return;
        }
    }
    QMessageBox::critical(this, tr("Can't load book"), tr("Unsupported file format or broken file!"));
}

void QBRMainWindow::bookLoadFinished(bool ok)
{
    if (!ok) return;

    findChild<QWebEngineView*>("browser")->page()->history()->clear();
    if (findChild<QWebEngineView*>("browser")->url().toLocalFile() != "")
    {
        QFile::remove(findChild<QWebEngineView*>("browser")->url().toLocalFile());
    }

    positionRestore();
}

void QBRMainWindow::positionSave()
{
    // If no file loaded
    if (statusBarFileName->text().length() < 1)
    {
        return;
    }
    // Save position
    qbrWebEnginePage* wp = (qbrWebEnginePage*)findChild<QWebEngineView*>("browser")->page();
    wp->positionSave(statusBarFileName->text());
}

void QBRMainWindow::positionRestore()
{
    // If no file loaded
    if (statusBarFileName->text().length() < 1)
    {
        return;
    }
    // Restore position
    qbrWebEnginePage* wp = (qbrWebEnginePage*)findChild<QWebEngineView*>("browser")->page();
    wp->positionRestore(statusBarFileName->text());
}


QBRMainWindow::~QBRMainWindow()
{
    delete ui;
}
