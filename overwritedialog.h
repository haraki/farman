#ifndef OVERWRITEDIALOG_H
#define OVERWRITEDIALOG_H

#include <QDialog>
#include "types.h"

namespace Ui {
class OverwriteDialog;
}

namespace Farman
{

class OverwriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OverwriteDialog(OverwriteMethodType methodType, const QString& renameText, QWidget *parent = Q_NULLPTR);
    ~OverwriteDialog();

    OverwriteMethodType getMethodType() const;
    QString getRenameFileName() const;

private slots:
    void on_methodOverwriteRadioButton_clicked();

    void on_methodOverwriteIfNewerRadioButton_clicked();

    void on_methodSkipRadioButton_clicked();

    void on_methodRenameRadioButton_clicked();

private:
    void accept() Q_DECL_OVERRIDE;

    Ui::OverwriteDialog *ui;

    OverwriteMethodType m_methodType;
    QString m_renameFileName;
};

}           // namespace Farman

#endif // OVERWRITEDIALOG_H
