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
                "<style type=\"text/css\">\n"
                "    body {\n"
                "        background: #ffeeaa;\n"
                "        color: #000000;\n"
                "        font-size: " + qbrcommon::getDesktopScaledIntAsQString(16) + "px;\n"
                "        font-family: Verdana, Arial, Helvetica, sans-serif;\n"
                "        word-wrap: break-word;\n"
                "    }\n"

                "   .doc_title {\n"
                "        font-size: " + qbrcommon::getDesktopScaledIntAsQString(22) + "px;\n"
                "        color: #222222;\n"
                "        font-weight: bold;\n"
                "        text-align: center;\n"
                "    }\n"

                "    .doc_subtitle {\n"
                "        font-size: " + qbrcommon::getDesktopScaledIntAsQString(18) + "px;\n"
                "        color: #444444;\n"
                "        font-weight: bold;\n"
                "        text-align: center;\n"
                "    }\n"

                "    .doc_poem {\n"
                "        color: #666666;\n"
                "        font-style: italic;\n"
                "    }\n"

                "    .doc_section {\n"
                "        margin-left: " + qbrcommon::getDesktopScaledIntAsQString(20) + "px;\n"
                "        margin-right: " + qbrcommon::getDesktopScaledIntAsQString(20) + "px;\n"
                "    }\n"

                "    .doc_note_link {\n"
                "        vertical-align: super;\n"
                "        font-size: " + qbrcommon::getDesktopScaledIntAsQString(12) + "px;\n"
                "    }\n"

                "    .doc_note_body {\n"
                "        font-size: 75%;\n"
                "    }\n"

                "    .doc_notes {\n"
                "    }\n"

                "    .doc_notes .doc_title {\n"
                "        text-align: left;\n"
                "    }\n"

                "    .doc_epigraph {\n"
                "        text-align: right;\n"
                "    }\n"

                "    .doc_annotation {\n"
                "        font-size: 75%;\n"
                "    }\n"

                "    img {\n"
                "        max-width: 90%;\n"
                "    }\n"

                "    .comics_image {\n"
                "        width: 100%;\n"
                "        text-align: center;\n"
                "    }\n"

                "   .amb_body {\n"
                "       font-family: 'Roboto Mono', monospace;\n"
                //"       white-space: pre;\n"
                "   }\n"

                "   span.amb_notice {\n"
                "       color: #333300;\n"
                "   }\n"

                "   span.amb_boring {\n"
                "       color: 888888;\n"
                "   }\n"

                "</style>\n"
                "</head>\n"
                "<body>\n"
                );
}

QString qbrtemplate::footer()
{
    return QString("</body>\n</html>\n");
}

QString qbrtemplate::empty()
{
    QString rv;
    rv.append(qbrtemplate::header());
    rv.append("<div class=\"document_body\">\n");
        rv.append("<div class=\"doc_title\"><p>Welcome to Qt Book Reader!</p></div>\n");
        rv.append(
                    "<div class=\"doc_section\">"
                    "<p>More info and source code can be found at "
                    "<a href=\"https://github.com/moose-kazan/qbr\">"
                    "https://github.com/moose-kazan/qbr"
                    "</a>"
                    "</p>"
                    "</div>"
                );
    rv.append("<div>\n");
    rv.append(qbrtemplate::footer());
    return rv;
}

QString qbrtemplate::emptyAsDataUri()
{
    QString rv = "data:text/html;base64,";
    rv.append(qbrtemplate::empty().toUtf8().toBase64());
    return rv;
}
