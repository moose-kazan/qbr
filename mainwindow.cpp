#include "mainwindow.h"
#include "qbrcfg.h"
#include "qbrtemplate.h"
#include "qbrformat.h"
#include "format/qbrformatcbz.h"
#include "format/qbrformatfb2.h"

#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QIcon>
#include <QToolBar>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    browser = new QTextBrowser(this);
    browser->setOpenExternalLinks(true);
    browser->setHtml(qbrtemplate::empty());
    setCentralWidget(browser);

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

    QMenu* helpMenu = menubar->addMenu(tr("&Help"));
    helpMenu->addAction(helpAboutAction);
    helpMenu->addAction(helpAboutQtAction);

    QToolBar* mainToolBar = addToolBar(tr("Main toolbar"));
    mainToolBar->setMovable(false);
    mainToolBar->addAction(QIcon::fromTheme("document-open"), tr("Open file"), this, &MainWindow::openFile);
    mainToolBar->addAction(QIcon::fromTheme("document-save-as"), tr("Save file as..."), this, &MainWindow::saveAsFile);
    mainToolBar->addSeparator();
    mainToolBar->addAction(QIcon::fromTheme("help-about"), tr("About"), this, &MainWindow::helpAbout);

    setWindowTitle(tr("Qt Book Reader"));
    setGeometry(300, 300, 480, 320);
    readSettings();
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."), tr(""), tr("Html pages (*.html *.html)"));
    if (fileName != "")
    {
        QFile f(fileName);
        if(f.open(QIODevice::WriteOnly))
        {
            f.write(browser->toHtml().toUtf8());
            f.close();
        }
        //browser->toHtml();
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), tr("Books (*.fb2 *.cbz)"));
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
        return;
    }

    QList<qbrformat*> parsers;
    parsers.append(new qbrformatfb2());
    parsers.append(new qbrformatcbz());
    for (int i = 0; i < parsers.count(); i++) {
        if (parsers.at(i)->loadFile(fileName, fileData)) {
            browser->setHtml(parsers.at(i)->getHtml());
            return;
        }
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

