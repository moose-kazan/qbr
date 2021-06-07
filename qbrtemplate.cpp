#include "qbrtemplate.h"

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
                "        font-size: 14pt;\n"
                "        word-wrap: break-word;\n"
                "    }\n"

                "   .doc_title {\n"
                "        font-size: 18pt;\n"
                "        color: #222222;\n"
                "        font-weight: bold;\n"
                "        text-align: center;\n"
                "    }\n"

                "    .doc_subtitle {\n"
                "        font-size: 16pt;\n"
                "        color: #444444;\n"
                "        font-weight: bold;\n"
                "        text-align: center;\n"
                "    }\n"

                "    .doc_poem {\n"
                "        color: #666666;\n"
                "        font-style: italic;\n"
                "    }\n"

                "    .doc_section {\n"
                "        margin-left: 50px;\n"
                "        margin-right: 50px;\n"
                "    }\n"

                "    .doc_note_link {\n"
                "        vertical-align: super;\n"
                "        font-size: 10pt;\n"
                "    }\n"

                "    img {\n"
                "        width: 90%;\n"
                "        max-width: 90%;\n"
                "    }\n"

                "    .comics_image {\n"
                "        display: block;"
                "    }\n"
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
    return qbrtemplate::header() + qbrtemplate::footer();
}
