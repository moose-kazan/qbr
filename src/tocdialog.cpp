//
// Created by moose on 03.11.2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_tocdialog.h" resolved

#include "tocdialog.h"

#include <QTableWidget>

#include "ui_tocdialog.h"


TocDialog::TocDialog(QWidget* parent, const Qt::WindowFlags f) :
    QDialog(parent, f), ui(new Ui::TocDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Table of contents"));
    setModal(true);
    setFixedSize(size());

    tocTreeView = findChild<QTreeView*>("tocTreeView");
    tocData = new QStandardItemModel();
    tocTreeView->setModel(tocData);
}

void TocDialog::setData(QList<QBRTocItem>* tocList)
{
    tocData->clear();

    QStandardItem *rootItem = tocData->invisibleRootItem();
    appendTocData(rootItem, tocList);
    tocTreeView->expandAll();
}

void TocDialog::appendTocData(QStandardItem* curItem, QList<QBRTocItem>* tocList)
{
    for (qsizetype i = 0; i < tocList->count(); i++)
    {
        auto *item = new QStandardItem(tocList->at(i).Title);
        item->setData(tocList->at(i).Anchor, Qt::UserRole+1);
        if (tocList->at(i).Childs.count() > 0)
        {
            auto childs = tocList->at(i).Childs;
            appendTocData(item, &childs);
        }
        curItem->appendRow(item);
    }
}

void TocDialog::gotoClick()
{
    if (tocTreeView->model()->rowCount() == 0)
    {
        reject();
        return;
    }
    accept();
}

void TocDialog::cancelClick()
{
    reject();
}

QString TocDialog::getSelectedAnchor() const
{
    const QModelIndex index = tocTreeView->currentIndex();
    auto data = tocTreeView->model()->itemData(index);
    return data[Qt::UserRole+1].toString();
}
