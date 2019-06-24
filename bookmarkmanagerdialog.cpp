#include <QMessageBox>
#include <QDebug>
#include "bookmarkinfo.h"
#include "bookmarkinfomodel.h"
#include "bookmarkmanagerdialog.h"
#include "ui_bookmarkmanagerdialog.h"
#include "bookmarkeditdialog.h"

namespace Farman
{

BookmarkManagerDialog::BookmarkManagerDialog(const QString& currentDirPath, QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::BookmarkManagerDialog),
    m_bookmarkInfoModel(new BookmarkInfoModel(false, false)),
    m_currentDirPath(currentDirPath)
{
    ui->setupUi(this);

    ui->upToolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarShadeButton));
    ui->downToolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));

    ui->bookmarksTableView->setModel(m_bookmarkInfoModel);
    ui->bookmarksTableView->setCurrentIndex(m_bookmarkInfoModel->index(0, 0));

    ui->bookmarksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

BookmarkManagerDialog::~BookmarkManagerDialog()
{
    delete m_bookmarkInfoModel;

    delete ui;
}

void BookmarkManagerDialog::accept()
{
    m_bookmarkInfoModel->saveToSettings();

    QDialog::accept();
}

void BookmarkManagerDialog::on_newPushButton_clicked()
{
    qDebug() << "BookmarkManagerDialog::on_newPushButton_clicked()";

    BookmarkInfo retInfo;
    if(BookmarkEditDialog::launchDialog(m_currentDirPath, BookmarkInfo(), retInfo, this) != QDialog::Accepted)
    {
        return;
    }

    qDebug() << "retInfo name : " << retInfo.getName() << ", path : " << retInfo.getPath();

    int row = m_bookmarkInfoModel->rowCount();
    qDebug() << "row : " << row;
    if(!m_bookmarkInfoModel->insertRows(row, 1))
    {
        qDebug() << "insert error.";

        return;
    }

    qDebug() << "insert success.";

    m_bookmarkInfoModel->setData(m_bookmarkInfoModel->index(row, 0), retInfo.getName());
    m_bookmarkInfoModel->setData(m_bookmarkInfoModel->index(row, 1), retInfo.getPath());
}

void BookmarkManagerDialog::on_editPushButton_clicked()
{
    qDebug() << "BookmarkManagerDialog::on_editPushButton_clicked()";

    QModelIndex index = ui->bookmarksTableView->currentIndex();
    if(!index.isValid())
    {
        return;
    }

    editBookmark(index);
}

void BookmarkManagerDialog::on_deletePushButton_clicked()
{
    qDebug() << "BookmarkManagerDialog::on_deletePushButton_clicked()";

    QModelIndex index = ui->bookmarksTableView->currentIndex();
    if(!index.isValid())
    {
        return;
    }

    if(QMessageBox::question(this,
                             tr("Confirm"),
                             tr("Are you sure you want to delete this bookmark?") + "<br><br>"
                             + tr("Name : ") + m_bookmarkInfoModel->data(index, BookmarkInfoModel::NameRole).toString() + "<br>"
                             + tr("Path : ") + m_bookmarkInfoModel->data(index, BookmarkInfoModel::PathRole).toString())
            != QMessageBox::Yes)
    {
        return;
    }

    m_bookmarkInfoModel->removeRow(index.row());
}

void BookmarkManagerDialog::on_upToolButton_clicked()
{
    qDebug() << "BookmarkManagerDialog::on_upToolButton_clicked()";

    QModelIndex index = ui->bookmarksTableView->currentIndex();
    if(!index.isValid() || index.row() <= 0)
    {
        return;
    }

    m_bookmarkInfoModel->moveRow(ui->bookmarksTableView->rootIndex(), index.row(), ui->bookmarksTableView->rootIndex(), index.row() - 1);
}

void BookmarkManagerDialog::on_downToolButton_clicked()
{
    qDebug() << "BookmarkManagerDialog::on_downToolButton_clicked()";

    QModelIndex index = ui->bookmarksTableView->currentIndex();
    if(!index.isValid() || index.row() >= m_bookmarkInfoModel->rowCount() - 1)
    {
        return;
    }

    m_bookmarkInfoModel->moveRow(ui->bookmarksTableView->rootIndex(), index.row(), ui->bookmarksTableView->rootIndex(), index.row() + 2);
}

void BookmarkManagerDialog::on_bookmarksTableView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }

    editBookmark(index);
}

void BookmarkManagerDialog::editBookmark(const QModelIndex& index)
{
    BookmarkInfo retInfo;
    if(BookmarkEditDialog::launchDialog(m_currentDirPath,
                                        {BookmarkInfo::Type::User,
                                         m_bookmarkInfoModel->data(index, BookmarkInfoModel::NameRole).toString(),
                                         m_bookmarkInfoModel->data(index, BookmarkInfoModel::PathRole).toString()},
                                        retInfo,
                                        this) != QDialog::Accepted)
    {
        return;
    }

    int row = index.row();
    m_bookmarkInfoModel->setData(m_bookmarkInfoModel->index(row, 0), retInfo.getName());
    m_bookmarkInfoModel->setData(m_bookmarkInfoModel->index(row, 1), retInfo.getPath());
}

}           // namespace Farman
