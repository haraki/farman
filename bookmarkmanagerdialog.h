#ifndef BOOKMARKMANAGERDIALOG_H
#define BOOKMARKMANAGERDIALOG_H

#include <QDialog>
#include "bookmarkinfo.h"
#include "bookmarkinfomodel.h"

namespace Ui {
class BookmarkManagerDialog;
}

namespace Farman
{

class BookmarkManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkManagerDialog(const QString& currentDirPath, QWidget *parent = Q_NULLPTR);
    ~BookmarkManagerDialog() Q_DECL_OVERRIDE;

    void accept() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void on_newPushButton_clicked();
    void on_editPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_upToolButton_clicked();
    void on_downToolButton_clicked();

    void on_bookmarksTableView_doubleClicked(const QModelIndex &index);

private:
    void editBookmark(const QModelIndex& index);

    Ui::BookmarkManagerDialog *ui;

    BookmarkInfoModel *m_bookmarkInfoModel;

    const QString m_currentDirPath;
};

}           // namespace Farman

#endif // BOOKMARKMANAGERDIALOG_H
