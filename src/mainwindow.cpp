#include "mainwindow.h"
#include "qbrwebenginepage.h"
#include "libs/settings.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QWebEngineHistory>
#include <QShortcut>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    bookInfo = new QBRBook();
    bookLoader = new BookLoader();
    bookSaver = new BookSaver();

    ui->setupUi(this);

    mainBrowser = findChild<QWebEngineView*>("browser");
    mainBrowser->setPage(new qbrWebEnginePage);
    mainBrowser->setContextMenuPolicy(Qt::NoContextMenu);
    connect(findChild<QWebEngineView*>("browser"), &QWebEngineView::loadFinished,
            this, &MainWindow::bookLoadFinished);

    statusBarFileName = new QLabel(this);
    statusBar()->addWidget(statusBarFileName);

    setContextMenuPolicy(Qt::NoContextMenu);

    setWindowTitle(tr("Qt Book Reader"));
    // setGeometry(150, 150, 640, 480);
    setMinimumSize(600, 400);
    readState();

    aboutDlg = new AboutDialog(this, Qt::Dialog);
    findDlg = new FindDialog(this, Qt::Dialog);
    fileInfoDlg = new FileInfoDialog(this, Qt::Dialog);
    settingsDlg = new SettingsDialog(this, Qt::Dialog);
    tocDlg = new TocDialog(this, Qt::Dialog);

    openFileDlg = new QFileDialog();

    readSettings();

    openFileDlg->setNameFilters(bookLoader->getFilter());
    openFileDlg->selectNameFilter(bookLoader->getFilter().last());

    // Other FileSave DIalog options
    openFileDlg->setWindowTitle(tr("Open File"));
    openFileDlg->setAcceptMode(QFileDialog::AcceptOpen);

    returnToMaximized = isMaximized();
    findChild<QAction *>("actionFullScreen")->setChecked(isFullScreen());

    const auto* shortcutFullscreen = new QShortcut(Qt::Key_F11, this);
    //shortcutFullscreen->setEnabled(false);
    connect(shortcutFullscreen, &QShortcut::activated, this, &MainWindow::toggleFullScreen);

}

void MainWindow::openFile()
{
    if (openFileDlg->exec() == QDialog::Accepted)
    {
        if (const QString fileName = openFileDlg->selectedFiles().at(0); fileName != "")
        {
            loadBook(fileName);
        }
    }
}

void MainWindow::saveFileAs()
{
    // If no file loaded
    if (getCurrentFileName().length() < 1)
    {
        return;
    }

    QString filterPath = getDefaultBookPath();
    QString selectedFilter;
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."),
                                                    filterPath, bookSaver->getFilter().join(";;"), &selectedFilter);

    if (fileName != "")
    {
        Export* exporter = bookSaver->exporterByFilter(selectedFilter);
        exporter->setData(bookInfo);
        exporter->save(fileName);
    }
}

void MainWindow::helpAbout() const { aboutDlg->show(); }

void MainWindow::helpAboutQt()
{
    QMessageBox::aboutQt(this, tr("Qt Book Reader"));
}

void MainWindow::naviGoBack() const
{
    mainBrowser->back();
}

void MainWindow::naviGoForward() const
{
    mainBrowser->forward();
}

void MainWindow::naviFind()
{
    if (getCurrentFileName().length() < 1)
    {
        return;
    }

    if (findDlg->exec() == QDialog::Accepted)
    {
        const QString findText = findDlg->findChild<QLineEdit*>("findText")->text();
        const bool optionFindBackward =
            findDlg->findChild<QCheckBox*>("findBackward")->isChecked();
        const bool optionCaseSensitive =
            findDlg->findChild<QCheckBox*>("CaseSensitive")->isChecked();

        QWebEnginePage::FindFlags findFlags;
        if (optionFindBackward)
            findFlags.setFlag(QWebEnginePage::FindBackward, true);
        if (optionCaseSensitive)
            findFlags.setFlag(QWebEnginePage::FindCaseSensitively, true);

        mainBrowser->findText(findText, findFlags);
    }
    else
    {
        mainBrowser->findText("");
    }
}

void MainWindow::settingsShow() const
{
    settingsDlg->settingsLoad();
    if (settingsDlg->exec() == QDialog::Accepted)
    {
        settingsDlg->settingsSave();
        readSettings();
        readBookSettings();
    }
}

void MainWindow::fileBookInfo()
{
    if (getCurrentFileName().length() < 1)
    {
        return;
    }
    fileInfoDlg->setBookInfo(bookInfo->metadata);
    if (fileInfoDlg->exec() == QDialog::Accepted)
    {

    }
}

void MainWindow::toggleFullScreen()
{
    if (!isFullScreen())
    {
        findChild<QAction *>("actionFullScreen")->setChecked(true);;
        returnToMaximized = isMaximized();
        showFullScreen();
    }
    else
    {
        findChild<QAction *>("actionFullScreen")->setChecked(false);
        if (returnToMaximized)
        {
            showMaximized();
        }
        else
        {
            showNormal();
        }
    }

    readSettings();
}

void MainWindow::showToc()
{
    tocDlg->setData(&bookInfo->metadata->Toc);
    if (tocDlg->exec() == QDialog::Accepted)
    {
        const QString anchor = tocDlg->getSelectedAnchor();
        auto* wp = dynamic_cast<qbrWebEnginePage*>(mainBrowser->page());
        wp->scrollToAnchor(anchor);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    positionSave();
    Settings::setMainWindowState(saveState());
    Settings::setMainWindowGeometry(saveGeometry());
    if (getCurrentFileName().length() > 0)
    {
        Settings::setLastOpenedFile(getCurrentFileName());
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::readBookSettings() const
{
    mainBrowser->page()->setBackgroundColor(
        QColor(Settings::getBookBgColor()));
}

void MainWindow::readState()
{
    restoreGeometry(Settings::getMainWindowGeometry(saveGeometry()));
    restoreState(Settings::getMainWindowState(saveState()));
}

void MainWindow::readSettings() const
{
    const bool isFS = isFullScreen();
    const bool hideUI = Settings::getHideUIOnFullScreen();
    const bool trueCalculated = !isFS || !hideUI;

    openFileDlg->setDirectory(getDefaultBookPath());
    statusBar()->setVisible(trueCalculated && Settings::getStatusBarEnabled());

    switch (Settings::getUiVariant())
    {
    case Settings::uiMenuOnly:
        menuBar()->setVisible(trueCalculated);
        findChild<QToolBar*>("toolBar")->setVisible(false);

        break;

    case Settings::uiToolbarOnly:
        menuBar()->setVisible(false);
        findChild<QToolBar*>("toolBar")->setVisible(trueCalculated);
        break;

    default:
        menuBar()->setVisible(trueCalculated);
        findChild<QToolBar*>("toolBar")->setVisible(trueCalculated);
    }
}

void MainWindow::loadBook(const QString& fileName)
{
    positionSave();

    try
    {
        if (bookLoader->loadFile(fileName))
        {
            QTemporaryFile f(QDir::tempPath() + "/QBR.XXXXXXX.html");
            f.setAutoRemove(false);
            if (f.open())
            {
                const QBRBook* fullBook = bookLoader->getBook();
                f.write(fullBook->html.toUtf8());
                f.close();

                mainBrowser->load(
                    QUrl::fromLocalFile(f.fileName()));
                setCurrentFileName(fileName);
                bookInfo->copy(fullBook);
            }
            return;
        }
    }
    catch (...)
    {
        QMessageBox::critical(this, tr("Can't load book"), tr("I/O error!"));
        return;
    }

    QMessageBox::critical(this, tr("Can't load book"),
                          tr("Unsupported file format or broken file!"));
}

void MainWindow::bookLoadFinished(const bool ok)
{
    if (!ok)
        return;

    mainBrowser->page()->history()->clear();
    if (mainBrowser->url().toLocalFile() != "")
    {
        QFile::remove(mainBrowser->url().toLocalFile());
    }

    readBookSettings();
    positionRestore();
}

void MainWindow::positionSave()
{
    // If no file loaded
    if (getCurrentFileName().length() < 1)
    {
        return;
    }
    // Save position
    const auto wp =
        dynamic_cast<qbrWebEnginePage*>(mainBrowser->page());
    wp->positionSave(getCurrentFileName());
}

void MainWindow::positionRestore()
{
    // If no file loaded
    if (getCurrentFileName().length() < 1)
    {
        return;
    }
    // Restore position
    const auto wp =
        dynamic_cast<qbrWebEnginePage*>(findChild<QWebEngineView*>("browser")->page());
    wp->positionRestore(getCurrentFileName());
}

QString MainWindow::getDefaultBookPath()
{
    int defaultPathType = Settings::getDefaultPath();
    QString defaultPathValue = Settings::getDefaultPathCustom();

    if (defaultPathType == Settings::defaultPathCustom && !defaultPathValue.isEmpty())
    {
        return defaultPathValue;
    }

    // Start path: by default - $HOME, but if exists - xdg-documents
    QString filterPath = QDir::homePath();
    if (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
        .count() > 0)
    {
        filterPath =
            QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
            .at(0);
    }

    return filterPath;
}

void MainWindow::setCurrentFileName(const QString& fileName)
{
    const QFileInfo fi(fileName);
    setWindowTitle(tr("Qt Book Reader - %1").arg(fi.fileName()));
    statusBarFileName->setText(fi.fileName());
    currentFileName = fileName;
}

QString MainWindow::getCurrentFileName() { return currentFileName; }

MainWindow::~MainWindow() { delete ui; }
