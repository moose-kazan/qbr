#include "template.h"

#include "libs/settings.h"

#include <QFile>
#include <QString>
#include <QTextStream>

Template::Template() {}

QString Template::header() {
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
  if (Settings::getCustomStyleEnabled() && Settings::getCustomStyleUrl() != "") {
    rv += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" +
          Settings::getCustomStyleUrl().toHtmlEscaped() + "\">";
  }
  rv += ("</head>\n"
         "<body>\n");
  return rv;
}

QString Template::footer() { return QString("</body>\n</html>\n"); }
