#ifndef BOOKMARKEDITDIALOG_H
#define BOOKMARKEDITDIALOG_H

#include <QDialog>
#include "bookmarkinfo.h"

namespace Ui {
class BookmarkEditDialog;
}

namespace Farman
{

class BookmarkEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkEditDialog(const QString& currentDirPath, const BookmarkInfo& info = BookmarkInfo(), QWidget *parent = Q_NULLPTR);
    ~BookmarkEditDialog() Q_DECL_OVERRIDE;

    void setBookmarkInfo(const BookmarkInfo& info) { m_info = info; }
    const BookmarkInfo& getBookmarkInfo() const { return m_info; }

    void accept() Q_DECL_OVERRIDE;

private slots:
    void on_nameLineEdit_textChanged(const QString &arg1);
    void on_selectPathToolButton_clicked();

private:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void checkNamePathEmpty();

    Ui::BookmarkEditDialog *ui;

    const QString m_currentDirPath;
    BookmarkInfo m_info;
};

}           // namespace Farman

#endif // BOOKMARKEDITDIALOG_H
