#include "qbrtemplate.h"

#include "qbrcfg.h"
#include "qbrcommon.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>

qbrtemplate::qbrtemplate() {}

QString qbrtemplate::header() {
  // Load CSS from resource
  QFile styleSheetFile(":/res/style.css");
  styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream styleSheetStream(&styleSheetFile);
  QString styleSheet = styleSheetStream.readAll();
  styleSheetFile.close();

  QString rv = ("<html>\n"
                "<head>\n"
                "<meta charset=\"utf-8\">\n"
                "<style type=\"text/css\">\n" +
                styleSheet + "</style>\n");
  if (qbrcfg::getCustomStyleEnabled() && qbrcfg::getCustomStyleUrl() != "") {
    rv += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" +
          qbrcfg::getCustomStyleUrl().toHtmlEscaped() + "\">";
  }
  rv += ("</head>\n"
         "<body>\n");
  return rv;
}

QString qbrtemplate::footer() { return QString("</body>\n</html>\n"); }
