#include "mainwindow.h"
#include "qbrcfg.h"
#include "qbrtemplate.h"
#include "qbrformat.h"
#include "qbrwebenginepage.h"
#include "format/qbrformatcbz.h"
#include "format/qbrformatfb2.h"
//#include "format/qbrformatfb3.h"

#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QIcon>
#include <QToolBar>
#include <QList>
#include <QDesktopWidget>
#include <QTemporaryFile>
#include <QDir>
#include <QWebEngineView>
#include <QWebEngineHistory>
#include <QLabel>
#include <QStatusBar>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    browser = new QWebEngineView(this);
    //browser->setOpenExternalLinks(true);
    browser->setPage(new qbrWebEnginePage);
    browser->setContextMenuPolicy(Qt::NoContextMenu);
    browser->load(QUrl(qbrtemplate::emptyAsDataUri()));
    setCentralWidget(browser);
    connect(browser, &QWebEngineView::loadFinished, this, &MainWindow::bookLoadFinished);

    QAction* openAction = new QAction(tr("Open file"), this);
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    openAction->setStatusTip(tr("Open file"));
    openAction->setIcon(QIcon::fromTheme("document-open"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    QAction* saveAsAction = new QAction(tr("Save as"), this);
    saveAsAction->setShortcut(QKeySequence("Ctrl+S"));
    saveAsAction->setStatusTip(tr("Save file as..."));
    saveAsAction->setIcon(QIcon::fromTheme("document-save-as"));
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);

    QAction* exitAction = new QAction(tr("Exit"), this);
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit application"));
    exitAction->setIcon(QIcon::fromTheme("application-exit"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);

    QAction* helpAboutAction = new QAction(tr("About"), this);
    helpAboutAction->setStatusTip(tr("About"));
    helpAboutAction->setIcon(QIcon::fromTheme("help-about"));
    connect(helpAboutAction, &QAction::triggered, this, &MainWindow::helpAbout);

    QAction* helpAboutQtAction = new QAction(tr("About Qt"), this);
    helpAboutQtAction->setStatusTip(tr("About Qt"));
    helpAboutQtAction->setIcon(QIcon::fromTheme("help-about"));
    connect(helpAboutQtAction, &QAction::triggered, this, &MainWindow::helpAboutQt);

    QMenuBar* menubar = this->menuBar();
    QMenu* fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(exitAction);

    QAction* naviActionBack = new QAction(tr("Go back"), this);
    naviActionBack->setStatusTip(tr("Go back"));
    naviActionBack->setIcon(QIcon::fromTheme("go-previous"));
    connect(naviActionBack, &QAction::triggered, this, &MainWindow::naviGoBack);

    QAction* naviActionForward = new QAction(tr("Go forward"), this);
    naviActionForward->setStatusTip(tr("Go forward"));
    naviActionForward->setIcon(QIcon::fromTheme("go-next"));
    connect(naviActionForward, &QAction::triggered, this, &MainWindow::naviGoForward);

    QAction* naviFind = new QAction(tr("Find text"), this);
    naviFind->setShortcut(QKeySequence("Ctrl+F"));
    naviFind->setStatusTip(tr("Find text"));
    naviFind->setIcon(QIcon::fromTheme("edit-find"));
    connect(naviFind, &QAction::triggered, this, &MainWindow::naviFind);

    QMenu* naviMenu = menubar->addMenu(tr("&Navigation"));
    naviMenu->addAction(naviActionBack);
    naviMenu->addAction(naviActionForward);
    naviMenu->addAction(naviFind);

    QMenu* helpMenu = menubar->addMenu(tr("&Help"));
    helpMenu->addAction(helpAboutAction);
    helpMenu->addAction(helpAboutQtAction);

    QToolBar* mainToolBar = addToolBar(tr("Main toolbar"));
    mainToolBar->setMovable(false);
    mainToolBar->addAction(QIcon::fromTheme("document-open"), tr("Open file"), this, &MainWindow::openFile);
    mainToolBar->addAction(QIcon::fromTheme("document-save-as"), tr("Save file as..."), this, &MainWindow::saveAsFile);
    mainToolBar->addSeparator();
    mainToolBar->addAction(QIcon::fromTheme("go-previous"), tr("Go back"), this, &MainWindow::naviGoBack);
    mainToolBar->addAction(QIcon::fromTheme("go-next"), tr("Go forward"), this, &MainWindow::naviGoForward);
    mainToolBar->addSeparator();
    mainToolBar->addAction(QIcon::fromTheme("edit-find"), tr("Find text"), this, &MainWindow::naviFind);
    mainToolBar->addSeparator();
    mainToolBar->addAction(QIcon::fromTheme("help-about"), tr("About"), this, &MainWindow::helpAbout);

    statusBarFileName = new QLabel(this);
    statusBar()->addWidget(statusBarFileName);

    setWindowTitle(tr("Qt Book Reader"));
    setGeometry(300, 300, 480, 320);
    readSettings();
}

void MainWindow::naviFind()
{
    // If file no loaded
    if (statusBarFileName->text().length() < 1)
    {
        return;
    }
    bool ok;
    QString text = QInputDialog::getText(
        this,
        tr("Find text"),
        tr("Text to find:"),
        QLineEdit::Normal,
        "",
        &ok,
        Qt::Dialog
    );

    if (ok)
    {
        browser->page()->findText(text);
    }
}

void MainWindow::naviGoBack()
{
    browser->back();
}

void MainWindow::naviGoForward()
{
    browser->forward();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings* cfg = qbrcfg::getInstance();
    cfg->setValue("geometry", saveGeometry());
    cfg->setValue("windowState", saveState());
    QMainWindow::closeEvent(event);

}

void MainWindow::readSettings()
{
    QSettings* cfg = qbrcfg::getInstance();
    restoreGeometry(cfg->value("geometry").toByteArray());
    restoreState(cfg->value("windowState").toByteArray());
}

void MainWindow::saveAsFile()
{
    // If file no loaded
    if (statusBarFileName->text().length() < 1)
    {
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), tr(""), tr("Html pages (*.html *.html)"));
    if (fileName != "")
    {
        browser->page()->toHtml([fileName](QString htmlData){
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

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), tr("Books (*.fb2 *.fb3 *.cbz)"));
    if (fileName != "")
    {
        loadBook(fileName);
    }
}

void MainWindow::loadBook(QString fileName)
{
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
    parsers.append(new qbrformatcbz());
    parsers.append(new qbrformatfb2());
    //parsers.append(new qbrformatfb3());
    for (int i = 0; i < parsers.count(); i++) {
        if (parsers.at(i)->loadFile(fileName, fileData)) {
            QTemporaryFile f(QDir::tempPath() + "/QBR.XXXXXXX.html");
            f.setAutoRemove(false);
            if(f.open())
            {
                f.write(parsers.at(i)->getHtml().toUtf8());
                f.close();

                browser->load(QUrl::fromLocalFile(f.fileName()));
                statusBarFileName->setText(fileName);
            }
            return;
        }
    }
    QMessageBox::critical(this, tr("Can't load book"), tr("Unsupported file format or broken file!"));
}

void MainWindow::bookLoadFinished(bool ok)
{
    if (!ok) return;

    // Fix for HiDpi screens
    // By default X.Org have 72 dpi
    QDesktopWidget desktop;
    if (desktop.logicalDpiY() > 72)
    {
        browser->setZoomFactor(desktop.logicalDpiY() / 72);
    }
    browser->page()->history()->clear();
    if (browser->url().toLocalFile() != "")
    {
        QFile::remove(browser->url().toLocalFile());
    }
}

void MainWindow::helpAbout()
{
    QMessageBox::about(this, tr("About Qt Book Reader"), tr("Simple Book reader written on C++ and using Qt5"));
}

void MainWindow::helpAboutQt()
{
    QMessageBox::aboutQt(this, tr("Qt Book Reader"));
}

MainWindow::~MainWindow()
{
}

