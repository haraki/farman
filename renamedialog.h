#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class RenameDialog;
}

namespace Farman
{

class RenameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(const QString& currentName, QWidget *parent = Q_NULLPTR);
    ~RenameDialog() Q_DECL_OVERRIDE;

    QString getNewName() const;

private:
    void accept() Q_DECL_OVERRIDE;

    Ui::RenameDialog *ui;

    QString m_newName;
};

}           // namespace Falman

#endif // RENAMEDIALOG_H
