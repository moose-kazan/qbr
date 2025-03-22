#include "libs/settings.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QString currentLocale = QLocale::system().name();
  currentLocale.truncate(currentLocale.lastIndexOf('_'));
  QString langFilePath = ":/languages/qbr_";
  langFilePath.append(currentLocale);
  langFilePath.append(".qm");

  // QIcon::setThemeSearchPaths(QStringList(":/icon"));
  // QIcon::setThemeName("qbr");

  QApplication a(argc, argv);

  QTranslator translator;
  if (translator.load(langFilePath)) {
    a.installTranslator(&translator);
  }

  a.setWindowIcon(QIcon(":/icon/64x64/qbr.png"));
  MainWindow w;
  w.show();

  /*
   * Default file from resources
   * Will be displayed if not file loaded
   */
  w.loadBook(a.translate("main.cpp", ":/res/default_fb2.xml"));
  if (argc > 1) {
    w.loadBook(argv[1]);
  } else if (Settings::getLastOpenedFileEnable() &&
             Settings::getLastOpenedFile().length() > 0) {
    w.loadBook(Settings::getLastOpenedFile());
  }
  return a.exec();
}
