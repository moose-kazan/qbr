#include "qbrformatamb.h"
#include "../qbrtemplate.h"

#include <QString>
#include <QtEndian>
#include <QUrl>
#include <QDebug>
#include <QCollator>

/*
 * AMB support
 *
 * Based on: http://amb.osdn.io/phpamb.php?fname=archiwum/format-20201216.amb
 *
 * Some ideas found here: https://sourceforge.net/p/utf8tocp/code/HEAD/tree/trunk/utf8tocp.c
 * And here: https://osdn.net/projects/amb/scm/svn/tree/head/phpamb/
 */

qbrformatamb::qbrformatamb()
{

}

bool qbrformatamb::parseAmb(QByteArray fileData)
{
    // Check file signature
    if (!fileData.startsWith("AMB1"))
    {
        return false;
    }
    int filesCount = qFromLittleEndian<qint16>(fileData.mid(4, 2).data());

    for (int i = 0; i < filesCount; i++)
    {
        QString entryName = "";
        int entryOffset = 6+i*20;
        for (int j = 0; j < 12 && fileData.at(entryOffset+j) != 0; j++)
        {
            entryName.append(fileData.at(entryOffset+j));
        }
        int entryFileOffset = qFromLittleEndian<qint32>(fileData.mid(entryOffset+12, 4).data());
        int entryFileLength = qFromLittleEndian<qint16>(fileData.mid(entryOffset+16, 2).data());

        ambEntries.insert(entryName.toLower(), fileData.mid(entryFileOffset, entryFileLength));

        //qDebug() << entryName << " " << entryFileOffset << " " << entryFileLength;

    }

    // If it have charmap
    if (ambEntries.contains("unicode.map")) {
        for (int i = 0; i < 128; i++)
        {
            unicodeMap[i] = qFromLittleEndian<qint16>(ambEntries.value("unicode.map").mid(i*2, 2).data());
        }

    }


    htmlData.append(qbrtemplate::header());

    htmlData.append(
                amaToHtml("index.ama")
            );

    // Sort entry names with QCollator. It's fix some issues fith numbers
    QStringList entryNames = ambEntries.keys();
    QCollator collator;
    collator.setNumericMode(true);

    for (int i = 0; i < entryNames.length() - 1; i++)
    {
        for (int j = i+1; j < entryNames.length(); j++)
        {
            int r = collator.compare(entryNames.at(i), entryNames.at(j));
            if (r > 0)
            {
                QString tmp = entryNames.at(i);
                entryNames.replace(i, entryNames.at(j));
                entryNames.replace(j, tmp);
            }
        }
    }

    for (int i = 0; i < entryNames.length(); i++)
    {
        if (entryNames.at(i) != "index.ama" && entryNames.at(i).endsWith(".ama"))
        {
            htmlData.append(
                        amaToHtml(entryNames.at(i))
                    );
        }
    }

    htmlData.append(qbrtemplate::footer());

    //htmlData = "Total files: " + QString::number(filesCount) + "!\n";


    return true;
}

QString qbrformatamb::convertToUtf8(QByteArray fileData)
{
    QString rv;
    for (int i = 0; i < fileData.size(); i++)
    {
        unsigned char cur_char = fileData[i];
        unsigned short new_char = 0;
        if (cur_char < 128)
        {
            rv.append(cur_char);
        }
        else
        {
            new_char = unicodeMap[cur_char-128];
            rv.append(new_char);
        }

    }
    return rv;
}

QString qbrformatamb::amaToHtml(QString fileName)
{
    QString rv;

    rv.append("<div class=\"amb_body\" id=\"");
    rv.append(fileName);
    rv.append("\">\n");

    rv.append("<div class=\"amb_part_header\">");
    rv.append("Part name: ");
    rv.append(fileName);
    rv.append(". <a href=\"#index.ama\">Go to index</a>.");
    rv.append("</div>\n");

    QString fileData = convertToUtf8(ambEntries.value(fileName, ""));

    bool readlink = false;
    bool escnow = false;
    QString opentag = "";

    for (int i = 0; i < fileData.length(); i++)
    {
        QChar c = fileData.at(i);
        if (c == "\r")
        {
            continue;
        }

        if (readlink)
        {
            if (c == "\n" || c == ":")
            {
                rv.append("\">");
                opentag = "a";
                readlink = false;

            }
            else
            {
                rv.append(QUrl::toPercentEncoding(c.toLower()));
            }
            continue;
        }

        if (escnow)
        {
            if (c == "%")
            {
                rv.append("%");
            }
            else if (c =="l")
            {
                rv.append("<a href=\"#");
                readlink = true;
            }
            else if (c == "h")
            {
                rv.append("<h1>");
                opentag = "h1";
            }
            else if (c == "!")
            {
                rv.append("<span class=\"amb_notice\">");
                opentag = "span";
            }
            else if (c == "b")
            {
                rv.append("<span class=\"amb_boring\">");
                opentag = "span";
            }

            escnow = false;
            continue;
        }

        if (opentag.length() > 0 && (c == "\n" || c == "%"))
        {
            // if it end with spaces
            for (int j = rv.length()-1; j >= 0; j--) {
                if (rv.at(j) != " ")
                {
                    rv.insert(j+1, "</" + opentag + ">");
                    break;
                }
            }
            opentag = "";
        }

        if (c == "%")
        {
            escnow = true;
        }
        else if (c == "\n")
        {
            rv.append("<br />\n");
        }
        else
        {
            rv.append(QString(c).toHtmlEscaped());
            //rv.append(c);
        }
    }

    if (opentag.length() > 0)
    {
        rv.append("</");
        rv.append(opentag);
        rv.append(">");
        opentag = "";
    }

    rv.append("</div>\n");
    rv.append("<hr />\n");

    return rv;
}

bool qbrformatamb::loadFile(QString fileName, QByteArray fileData)
{
    /*
     * AMB file have hard-defined signature. We use it to detect
     *
    QRegExp rx("\\.amb$", Qt::CaseInsensitive);
    if (rx.indexIn(fileName) < 0)
    {
        return false;
    }
    */
    (void)fileName; // Remove "unused parameter" warning
    try
    {
        return parseAmb(fileData);
    }
    catch (...)
    {
        return false;
    }

    return false;
}

QString qbrformatamb::getHtml()
{
    return htmlData;
}
