#include "qbrtemplate.h"

#include "qbrcommon.h"
#include "qbrcfg.h"

#include <QString>

qbrtemplate::qbrtemplate()
{

}

QString qbrtemplate::header()
{
    QString rv = (
                "<html>\n"
                "<head>\n"
                "<meta charset=\"utf-8\">\n"
                "<link rel=\"stylesheet\" type=\"text/css\" href=\"qrc:/res/style.css\">"
                );
    if (qbrcfg::getCustomStyleEnabled() && qbrcfg::getCustomStyleUrl() != "") {
        rv += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" +
                qbrcfg::getCustomStyleUrl().toHtmlEscaped() +
                "\">";
    }
    rv += (
                "</head>\n"
                "<body>\n"
                );
    return rv;
}

QString qbrtemplate::footer()
{
    return QString("</body>\n</html>\n");
}

