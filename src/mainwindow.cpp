#include "mainwindow.h"
#include "qbrwebenginepage.h"
#include "libs/settings.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QWebEngineHistory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  bookLoader = new BookLoader();
  bookSaver = new BookSaver();

  ui->setupUi(this);

  QWebEngineView *browser = findChild<QWebEngineView *>("browser");
  browser->setPage(new qbrWebEnginePage);
  browser->setContextMenuPolicy(Qt::NoContextMenu);
  connect(findChild<QWebEngineView *>("browser"), &QWebEngineView::loadFinished,
          this, &MainWindow::bookLoadFinished);

  statusBarFileName = new QLabel(this);
  statusBar()->addWidget(statusBarFileName);

  setContextMenuPolicy(Qt::NoContextMenu);

  setWindowTitle(tr("Qt Book Reader"));
  // setGeometry(150, 150, 640, 480);
  setMinimumSize(600, 400);
  readState();
  readSettings();

  aboutDlg = new AboutDialog(this, Qt::Dialog);
  findDlg = new FindDialog(this, Qt::Dialog);
  fileInfoDlg = new FileInfoDialog(this, Qt::Dialog);
  settingsDlg = new SettingsDialog(this, Qt::Dialog);

  openFileDlg = new QFileDialog();

  // Start path: by default - $HOME, but if exists - xdg-documents
  QString filterPath = QDir::homePath();
  if (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
          .count() > 0) {
    filterPath =
        QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
            .at(0);
  }
  openFileDlg->setDirectory(filterPath);

  openFileDlg->setNameFilters(bookLoader->getFilter());
  openFileDlg->selectNameFilter(bookLoader->getFilter().last());

  // Other FileSave DIalog options
  openFileDlg->setWindowTitle(tr("Open File"));
  openFileDlg->setAcceptMode(QFileDialog::AcceptOpen);
}

void MainWindow::openFile() {
  if (openFileDlg->exec() == QDialog::Accepted) {
    QString fileName = openFileDlg->selectedFiles().at(0);
    if (fileName != "") {
      loadBook(fileName);
    }
  }
}

void MainWindow::saveFileAs() {
    // If no file loaded
    if (getCurrentFileName().length() < 1) {
        return;
    }

    // Start path: by default - $HOME, but if exists - xdg-documents
    QString filterPath = QDir::homePath();
    if (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).count() > 0) {
        filterPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0);
    }

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."),
        filterPath, bookSaver->getFilter().join(";;"), &selectedFilter);

    if (fileName != "") {
        Export *exporter = bookSaver->exporterByFilter(selectedFilter);
        findChild<QWebEngineView *>("browser")->page()->toHtml([fileName, exporter](const QString&  htmlData) {
            QFile f(fileName);
            if (f.open(QIODevice::WriteOnly)) {
                f.write(exporter->fromHtml(htmlData).toUtf8());
                f.close();
            }
        });
        // browser->toHtml();
    }
}

void MainWindow::helpAbout() const { aboutDlg->show(); }

void MainWindow::helpAboutQt() {
  QMessageBox::aboutQt(this, tr("Qt Book Reader"));
}

void MainWindow::naviGoBack() const
{
  findChild<QWebEngineView *>("browser")->back();
}

void MainWindow::naviGoForward() const
{
  findChild<QWebEngineView *>("browser")->forward();
}

void MainWindow::naviFind() {
  if (getCurrentFileName().length() < 1) {
    return;
  }

  if (findDlg->exec() == QDialog::Accepted) {
    QString findText = findDlg->findChild<QLineEdit *>("findText")->text();
    bool optionFindBackward =
        findDlg->findChild<QCheckBox *>("findBackward")->isChecked();
    bool optionCaseSensitive =
        findDlg->findChild<QCheckBox *>("CaseSensitive")->isChecked();

    QWebEnginePage::FindFlags findFlags;
    if (optionFindBackward)
      findFlags.setFlag(QWebEnginePage::FindBackward, true);
    if (optionCaseSensitive)
      findFlags.setFlag(QWebEnginePage::FindCaseSensitively, true);

    findChild<QWebEngineView *>("browser")->findText(findText, findFlags);
  } else {
    findChild<QWebEngineView *>("browser")->findText("");
  }
}

void MainWindow::settingsShow() const
{
  settingsDlg->settingsLoad();
  if (settingsDlg->exec() == QDialog::Accepted) {
    settingsDlg->settingsSave();
    readSettings();
    readBookSettings();
  }
}

void MainWindow::fileBookInfo() {
  if (getCurrentFileName().length() < 1) {
    return;
  }
  fileInfoDlg->setBookInfo(bookInfo);
  fileInfoDlg->exec();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  positionSave();
  Settings::setMainWindowState(saveState());
  Settings::setMainWindowGeometry(saveGeometry());
  if (getCurrentFileName().length() > 0) {
    Settings::setLastOpenedFile(getCurrentFileName());
  }

  QMainWindow::closeEvent(event);
}

void MainWindow::readBookSettings() const
{
  findChild<QWebEngineView *>("browser")->page()->setBackgroundColor(
      QColor(Settings::getBookBgColor()));
}

void MainWindow::readState() {
  restoreGeometry(Settings::getMainWindowGeometry(saveGeometry()));
  restoreState(Settings::getMainWindowState(saveState()));
}
void MainWindow::readSettings() const
{
  statusBar()->setVisible(Settings::getStatusBarEnabled());

  switch (Settings::getUiVariant()) {
  case Settings::uiMenuOnly:
    menuBar()->setVisible(true);
    findChild<QToolBar *>("toolBar")->setVisible(false);

    break;

  case Settings::uiToolbarOnly:
    menuBar()->setVisible(false);
    findChild<QToolBar *>("toolBar")->setVisible(true);
    break;

  default:
    menuBar()->setVisible(true);
    findChild<QToolBar *>("toolBar")->setVisible(true);
  }
}

void MainWindow::loadBook(const QString& fileName) {
  positionSave();

  QByteArray fileData;
  try {
      if (bookLoader->loadFile(fileName)) {
          QTemporaryFile f(QDir::tempPath() + "/QBR.XXXXXXX.html");
          f.setAutoRemove(false);
          if (f.open()) {
              QBRBook fullBook = bookLoader->getBook();
              f.write(fullBook.html.toUtf8());
              f.close();

              findChild<QWebEngineView *>("browser")->load(
                  QUrl::fromLocalFile(f.fileName()));
              setCurrentFileName(fileName);
              bookInfo = fullBook.metadata;
          }
          return;
      }
  } catch (...) {
    QMessageBox::critical(this, tr("Can't load book"), tr("I/O error!"));
    return;
  }

  QMessageBox::critical(this, tr("Can't load book"),
                        tr("Unsupported file format or broken file!"));
}

void MainWindow::bookLoadFinished(const bool ok) {
  if (!ok)
    return;

  findChild<QWebEngineView *>("browser")->page()->history()->clear();
  if (findChild<QWebEngineView *>("browser")->url().toLocalFile() != "") {
    QFile::remove(findChild<QWebEngineView *>("browser")->url().toLocalFile());
  }

  readBookSettings();
  positionRestore();
}

void MainWindow::positionSave() {
  // If no file loaded
  if (getCurrentFileName().length() < 1) {
    return;
  }
  // Save position
  qbrWebEnginePage *wp =
      static_cast<qbrWebEnginePage*>(findChild<QWebEngineView*>("browser")->page());
  wp->positionSave(getCurrentFileName());
}

void MainWindow::positionRestore() {
  // If no file loaded
  if (getCurrentFileName().length() < 1) {
    return;
  }
  // Restore position
  qbrWebEnginePage *wp =
      static_cast<qbrWebEnginePage*>(findChild<QWebEngineView*>("browser")->page());
  wp->positionRestore(getCurrentFileName());
}

void MainWindow::setCurrentFileName(const QString& fileName) {
  const QFileInfo fi(fileName);
  setWindowTitle(tr("Qt Book Reader - %1").arg(fi.fileName()));
  statusBarFileName->setText(fi.fileName());
  currentFileName = fileName;
}
QString MainWindow::getCurrentFileName() { return currentFileName; }

MainWindow::~MainWindow() { delete ui; }
