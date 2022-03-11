#include "aboutdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

AboutDialog::AboutDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    QVBoxLayout *rootVLayout = new QVBoxLayout();
    QHBoxLayout *mainLayout = new QHBoxLayout();

    QLabel *appLogo = new QLabel();
    appLogo->setPixmap(QPixmap(":icon/128x128/qbr.png"));
    mainLayout->addWidget(appLogo);


    QString appTitleLine = "<h1>Qt Book Reader</h1>\n"
            "<p>Simple book reader written on C++ with Qt Library.</p>\n"
            "<p>For now it support formats:</p>\n"
            "<ul>\n"
            "<li>FictionBook 2.0 (fb2)</li>\n"
            "<li>FictionBook 3.0 (fb3)</li>\n"
            "<li>Comics Book Zip (cbz)</li>\n"
            "<li>Ancient Machine Book (amb)</li>\n"
            "</ul>\n"
            "<p>More info can be fount at <a href=\"https://github.com/moose-kazan/qbr\">https://github.com/moose-kazan/qbr</a>.</p>\n";

    QLabel *appTitle = new QLabel();
    appTitle->setTextFormat(Qt::RichText);
    appTitle->setText(appTitleLine);
    appTitle->setOpenExternalLinks(true);
    mainLayout->addWidget(appTitle);

    rootVLayout->addLayout(mainLayout);

    QPushButton *okButton = new QPushButton("OK", this);
    rootVLayout->addWidget(okButton);

    setLayout(rootVLayout);

    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

    //setWindowModality(Qt::ApplicationModal);
    setModal(true);
    setFixedSize(sizeHint());

    setWindowTitle("About Qt Book Reader");
}
