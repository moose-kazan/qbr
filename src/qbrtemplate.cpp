#include "qbrtemplate.h"

#include "qbrcommon.h"

#include <QString>

qbrtemplate::qbrtemplate()
{

}

QString qbrtemplate::header()
{
    return QString(
                "<html>\n"
                "<head>\n"
                "<meta charset=\"utf-8\">\n"
                "<link rel=\"stylesheet\" type=\"text/css\" href=\"qrc:/res/style.css\">"
                "</head>\n"
                "<body>\n"
                );
}

QString qbrtemplate::footer()
{
    return QString("</body>\n</html>\n");
}

