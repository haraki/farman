#ifndef SELECTBOOKMARKDIALOG_H
#define SELECTBOOKMARKDIALOG_H

#include <QDialog>
#include "bookmarkinfomodel.h"

namespace Ui {
class SelectBookmarkDialog;
}

namespace Farman
{

class SelectBookmarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectBookmarkDialog(QWidget *parent = Q_NULLPTR);
    ~SelectBookmarkDialog();

    const QString& getSelectedPath() { return m_selectedPath; }

    void accept();

private:
    Ui::SelectBookmarkDialog *ui;

    BookmarkInfoModel *m_bookmarkInfoModel;

    QString m_selectedPath;
};

}           // namespace Farman

#endif // SELECTBOOKMARKDIALOG_H
