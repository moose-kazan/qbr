#include "qbrmainwindow.h"
#include "qbraboutdialog.h"
#include "qbrcfg.h"
#include "qbrsettingsdialog.h"
#include "qbrtemplate.h"
#include "qbrwebenginepage.h"
#include "ui_qbrmainwindow.h"

#include "format/qbrformat.h"
#include "format/qbrformatamb.h"
#include "format/qbrformatcbz.h"
#include "format/qbrformatfb2.h"
#include "format/qbrformatfb3.h"

#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QWebEngineHistory>
#include <QRegularExpression>

QBRMainWindow::QBRMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QBRMainWindow) {

  bookParsers.append(new QBRFormatAMB());
  bookParsers.append(new QBRFormatCBZ());
  bookParsers.append(new QBRFormatFB2());
  bookParsers.append(new QBRFormatFB3());

  ui->setupUi(this);

  findChild<QWidget *>("findWidget")->setVisible(false);

  QWebEngineView *browser = findChild<QWebEngineView *>("browser");
  browser->setPage(new qbrWebEnginePage);
  browser->setContextMenuPolicy(Qt::NoContextMenu);
  connect(findChild<QWebEngineView *>("browser"), &QWebEngineView::loadFinished,
          this, &QBRMainWindow::bookLoadFinished);

  statusBarFileName = new QLabel(this);
  statusBar()->addWidget(statusBarFileName);

  setContextMenuPolicy(Qt::NoContextMenu);

  setWindowTitle(tr("Qt Book Reader"));
  setGeometry(300, 300, 720, 600);
  readState();
  readSettings();
}

void QBRMainWindow::openFile() {
  // Start path: by default - $HOME, but if exists - xdg-documents
  QString filterPath = QDir::homePath();
  if (QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
          .count() > 0) {
    filterPath =
        QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)
            .at(0);
  }

  // All extensiosn from parsers
  QStringList allExt;
  for (int i = 0; i < bookParsers.count(); i++) {
    allExt.append(bookParsers.at(i)->getExtensions());
  }
  allExt.replaceInStrings(QRegularExpression("^"), "*.");

  // Filter line
  QString filterLine = QString(tr("Books (%1)")).arg(allExt.join(" "));
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                  filterPath, filterLine);
  if (fileName != "") {
    loadBook(fileName);
  }
}

void QBRMainWindow::saveFileAs() {
  // If file no loaded
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

void QBRMainWindow::helpAbout() {
  QBRAboutDialog *aboutDlg = new QBRAboutDialog(this, Qt::Dialog);
  aboutDlg->show();
}

void QBRMainWindow::helpAboutQt() {
  QMessageBox::aboutQt(this, tr("Qt Book Reader"));
}

void QBRMainWindow::naviGoBack() {
  findChild<QWebEngineView *>("browser")->back();
}

void QBRMainWindow::naviGoForward() {
  findChild<QWebEngineView *>("browser")->forward();
}

void QBRMainWindow::naviFindGo() {
  findChild<QWebEngineView *>("browser")->findText(
      findChild<QLineEdit *>("findText")->text());
}

void QBRMainWindow::naviFind() {
  if (getCurrentFileName().length() < 1) {
    return;
  }
  bool visible = findChild<QWidget *>("findWidget")->isVisible();

  QSize sizeMin = minimumSize();
  QSize sizeMax = maximumSize();
  setFixedSize(size());

  if (visible) {
    findChild<QWebEngineView *>("browser")->findText("");
    findChild<QLineEdit *>("findText")->setText("");
    findChild<QWidget *>("findWidget")->hide();
  } else {
    findChild<QWidget *>("findWidget")->show();
    findChild<QLineEdit *>("findText")->setFocus();
  }
  setMinimumSize(sizeMin);
  setMaximumSize(sizeMax);
}

void QBRMainWindow::settingsShow() {
  QBRSettingsDialog *settingsDlg = new QBRSettingsDialog(this, Qt::Dialog);
  settingsDlg->settingsLoad();
  if (settingsDlg->exec() == QDialog::Accepted) {
    settingsDlg->settingsSave();
    readSettings();
    readBookSettings();
  }
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
    QFile f(fileName);
    f.open(QIODevice::ReadOnly);
    QDataStream in(&f);
    int buff_size = 4096;
    char buff[buff_size];
    while (!in.atEnd()) {
      int readed = in.readRawData(buff, buff_size);
      if (readed > 0) {
        fileData.append(buff, readed);
      }
    }
    f.close();
  } catch (...) {
    QMessageBox::critical(this, tr("Can't load book"), tr("I/O error!"));
    return;
  }

  for (int i = 0; i < bookParsers.count(); i++) {
    if (bookParsers.at(i)->loadFile(fileName, fileData)) {
      QTemporaryFile f(QDir::tempPath() + "/QBR.XXXXXXX.html");
      f.setAutoRemove(false);
      if (f.open()) {
        f.write(bookParsers.at(i)->getHtml().toUtf8());
        f.close();

        findChild<QWebEngineView *>("browser")->load(
            QUrl::fromLocalFile(f.fileName()));
        setCurrentFileName(fileName);
      }
      return;
    }
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
