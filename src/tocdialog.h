//
// Created by moose on 03.11.2025.
//

#ifndef QBR_TOCDIALOG_H
#define QBR_TOCDIALOG_H

#include <QDialog>
#include <QTreeView>

#include "libs/qbrbookinfo.h"

namespace Ui
{
    class TocDialog;
}

class TocDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TocDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void setData(QList<QBRTocItem>* tocList);
    QString getSelectedAnchor() const;
private:
    Ui::TocDialog* ui;
    QTreeView* tocTreeView;
    QStandardItemModel* tocData;
    void appendTocData(QStandardItem* curItem, QList<QBRTocItem>* tocList);
private slots:
    void gotoClick();
    void cancelClick();
};


#endif //QBR_TOCDIALOG_H