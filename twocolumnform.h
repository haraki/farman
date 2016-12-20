#ifndef TWOCOLUMNFORM_H
#define TWOCOLUMNFORM_H

#include <QWidget>
#include <qdir.h>

namespace Ui {
class TwoColumnForm;
}
class QString;
class FolderForm;

class TwoColumnForm : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColumnForm(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = 0);
    ~TwoColumnForm();

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void setActiveFolderForm(const QString& objectName);
    FolderForm* getActiveFolderForm();

    Ui::TwoColumnForm *ui;
};

#endif // TWOCOLUMNFORM_H
