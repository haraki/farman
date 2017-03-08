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

private:
    Ui::OverwriteDialog *ui;

    OverwriteMethodType m_methodType;
    QString m_renameFileName;
};

}           // namespace Farman

#endif // OVERWRITEDIALOG_H
