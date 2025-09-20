#include "fileinfodialog.h"
#include "ui_fileinfodialog.h"

FileInfoDialog::FileInfoDialog(QWidget *parent, const Qt::WindowFlags f)
    : QDialog(parent, f), ui(new Ui::FileInfoDialog) {

  ui->setupUi(this);
  setWindowTitle(tr("Book Info"));
  setModal(true);
  setFixedSize(size());
}

void FileInfoDialog::setBookInfo(const qbrbookinfo& newBookInfo) {
  bookInfo = newBookInfo;
}

void FileInfoDialog::showEvent(QShowEvent *event) {
  (void)event;
  auto *tw = findChild<QTableWidget *>("bookInfo");

  tw->clear();
  tw->setRowCount(0); // Workaround for some bugly systems

  tw->setColumnCount(1);
  tw->horizontalHeader()->hide();

  tw->insertRow(0);
  tw->setVerticalHeaderItem(0, new QTableWidgetItem(tr("File type")));
  tw->setItem(0, 0, new QTableWidgetItem(bookInfo.FileFormat));

  tw->insertRow(1);
  tw->setVerticalHeaderItem(1, new QTableWidgetItem(tr("Book author")));
  tw->setItem(1, 0, new QTableWidgetItem(bookInfo.Author));

  tw->insertRow(2);
  tw->setVerticalHeaderItem(2, new QTableWidgetItem(tr("Book title")));
  tw->setItem(2, 0, new QTableWidgetItem(bookInfo.Title));

  tw->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

  for (int i = 0; i < 3; i++) {
    tw->item(i, 0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  }

  auto *cw = findChild<QLabel *>("bookCover");
  QPixmap coverPixmap;
  if (!bookInfo.Cover.isNull()) {
      coverPixmap.convertFromImage(bookInfo.Cover);
  }
  else {
      coverPixmap.load(":/icon/512x512/qbr.png");
  }
  cw->setPixmap(coverPixmap.scaled(cw->width(), cw->height(), Qt::KeepAspectRatio));
  //cw->setScaledContents(true);
  //qDebug() << cover.width() << "x" << cover.height() << cover.isNull();
}

FileInfoDialog::~FileInfoDialog() { delete ui; }
