#include "qbrmainwindow.h"
#include "qbraboutdialog.h"
#include "qbrcfg.h"
#include "qbrsettingsdialog.h"
#include "qbrwebenginepage.h"
#include "ui_qbrmainwindow.h"

#include "format/qbrformat.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QWebEngineHistory>

QBRMainWindow::QBRMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QBRMainWindow) {

  bookLoader = new QBRBookLoader();

  ui->setupUi(this);

  QWebEngineView *browser = findChild<QWebEngineView *>("browser");
  browser->setPage(new qbrWebEnginePage);
  browser->setContextMenuPolicy(Qt::NoContextMenu);
  connect(findChild<QWebEngineView *>("browser"), &QWebEngineView::loadFinished,
          this, &QBRMainWindow::bookLoadFinished);

  statusBarFileName = new QLabel(this);
  statusBar()->addWidget(statusBarFileName);

  setContextMenuPolicy(Qt::NoContextMenu);

  setWindowTitle(tr("Qt Book Reader"));
  // setGeometry(150, 150, 640, 480);
  setMinimumSize(600, 400);
  readState();
  readSettings();

  aboutDlg = new QBRAboutDialog(this, Qt::Dialog);
  findDlg = new QBRFindDialog(this, Qt::Dialog);
  fileInfoDlg = new QBRFileInfoDialog(this, Qt::Dialog);
  settingsDlg = new QBRSettingsDialog(this, Qt::Dialog);

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

  // All extensiosn from parsers
  QStringList allExt = bookLoader->getExtensions();
  allExt.replaceInStrings(QRegularExpression("^"), "*.");

  // Filter line
  QString filterLine = QString(tr("Books (%1)")).arg(allExt.join(" "));
  openFileDlg->setNameFilters(QStringList(filterLine));

  // Other FileSave DIalog options
  openFileDlg->setWindowTitle(tr("Open File"));
  openFileDlg->setAcceptMode(QFileDialog::AcceptOpen);
}

void QBRMainWindow::openFile() {
  if (openFileDlg->exec() == QDialog::Accepted) {
    QString fileName = openFileDlg->selectedFiles().at(0);
    if (fileName != "") {
      loadBook(fileName);
    }
  }
}

void QBRMainWindow::saveFileAs() {
  // If no file loaded
  if (getCurrentFileName().length() < 1) {
    return;
  }

  // Start path: by default - $HOME, but if exists - xdg-documents
  QString filterPath = QDir::homePath();
  if (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
          .count() > 0) {
    filterPath =
        QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
            .at(0);
  }

  // Filter line
  QString filterLine = QString(tr("Html pages (%1)")).arg("*.htm *.html");

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save file as..."),
                                                  filterPath, filterLine);
  if (fileName != "") {
    findChild<QWebEngineView *>("browser")->page()->toHtml(
        [fileName](QString htmlData) {
          QFile f(fileName);
          if (f.open(QIODevice::WriteOnly)) {
            f.write(htmlData.toUtf8());
            f.close();
          }
        });
    // browser->toHtml();
  }
}

void QBRMainWindow::helpAbout() { aboutDlg->show(); }

void QBRMainWindow::helpAboutQt() {
  QMessageBox::aboutQt(this, tr("Qt Book Reader"));
}

void QBRMainWindow::naviGoBack() {
  findChild<QWebEngineView *>("browser")->back();
}

void QBRMainWindow::naviGoForward() {
  findChild<QWebEngineView *>("browser")->forward();
}

void QBRMainWindow::naviFind() {
  if (getCurrentFileName().length() < 1) {
    return;
  }

  if (findDlg->exec() == QDialog::Accepted) {
    QString findText = findDlg->findChild<QLineEdit *>("findText")->text();
    bool optionFindBackward =
        findDlg->findChild<QCheckBox *>("findBackward")->isChecked();
    bool optionCaseSentive =
        findDlg->findChild<QCheckBox *>("CaseSensitive")->isChecked();

    QWebEnginePage::FindFlags findFlags;
    if (optionFindBackward)
      findFlags.setFlag(QWebEnginePage::FindBackward, true);
    if (optionCaseSentive)
      findFlags.setFlag(QWebEnginePage::FindCaseSensitively, true);

    findChild<QWebEngineView *>("browser")->findText(findText, findFlags);
  } else {
    findChild<QWebEngineView *>("browser")->findText("");
  }
}

void QBRMainWindow::settingsShow() {
  settingsDlg->settingsLoad();
  if (settingsDlg->exec() == QDialog::Accepted) {
    settingsDlg->settingsSave();
    readSettings();
    readBookSettings();
  }
}

void QBRMainWindow::fileBookInfo() {
  if (getCurrentFileName().length() < 1) {
    return;
  }
  fileInfoDlg->setBookInfo(bookInfo);
  fileInfoDlg->exec();
}

void QBRMainWindow::closeEvent(QCloseEvent *event) {
  positionSave();
  qbrcfg::setMainWindowState(saveState());
  qbrcfg::setMainWindowGeometry(saveGeometry());
  if (getCurrentFileName().length() > 0) {
    qbrcfg::setLastOpenedFile(getCurrentFileName());
  }

  QMainWindow::closeEvent(event);
}

void QBRMainWindow::readBookSettings() {
  findChild<QWebEngineView *>("browser")->page()->setBackgroundColor(
      QColor(qbrcfg::getBookBgColor()));
}

void QBRMainWindow::readState() {
  restoreGeometry(qbrcfg::getMainWindowGeometry(saveGeometry()));
  restoreState(qbrcfg::getMainWindowState(saveState()));
}
void QBRMainWindow::readSettings() {
  statusBar()->setVisible(qbrcfg::getStatusBarEnabled());

  switch (qbrcfg::getUiVariant()) {
  case qbrcfg::uiMenuOnly:
    menuBar()->setVisible(true);
    findChild<QToolBar *>("toolBar")->setVisible(false);

    break;

  case qbrcfg::uiToolbarOnly:
    menuBar()->setVisible(false);
    findChild<QToolBar *>("toolBar")->setVisible(true);
    break;

  default:
    menuBar()->setVisible(true);
    findChild<QToolBar *>("toolBar")->setVisible(true);
  }
}

void QBRMainWindow::loadBook(QString fileName) {
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

void QBRMainWindow::bookLoadFinished(bool ok) {
  if (!ok)
    return;

  findChild<QWebEngineView *>("browser")->page()->history()->clear();
  if (findChild<QWebEngineView *>("browser")->url().toLocalFile() != "") {
    QFile::remove(findChild<QWebEngineView *>("browser")->url().toLocalFile());
  }

  readBookSettings();
  positionRestore();
}

void QBRMainWindow::positionSave() {
  // If no file loaded
  if (getCurrentFileName().length() < 1) {
    return;
  }
  // Save position
  qbrWebEnginePage *wp =
      (qbrWebEnginePage *)findChild<QWebEngineView *>("browser")->page();
  wp->positionSave(getCurrentFileName());
}

void QBRMainWindow::positionRestore() {
  // If no file loaded
  if (getCurrentFileName().length() < 1) {
    return;
  }
  // Restore position
  qbrWebEnginePage *wp =
      (qbrWebEnginePage *)findChild<QWebEngineView *>("browser")->page();
  wp->positionRestore(getCurrentFileName());
}

void QBRMainWindow::setCurrentFileName(QString fileName) {
  QFileInfo fi(fileName);
  setWindowTitle(tr("Qt Book Reader - %1").arg(fi.fileName()));
  statusBarFileName->setText(fi.fileName());
  currentFileName = fileName;
}
QString QBRMainWindow::getCurrentFileName() { return currentFileName; }

QBRMainWindow::~QBRMainWindow() { delete ui; }
