#ifndef DOUBLEPANELFORM_H
#define DOUBLEPANELFORM_H

#include <QWidget>
#include <qdir.h>

namespace Ui {
class DoublePanelForm;
}
class QString;
class FolderForm;

class DoublePanelForm : public QWidget
{
    Q_OBJECT

public:
    explicit DoublePanelForm(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = Q_NULLPTR);
    ~DoublePanelForm();

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void setActiveFolderForm(const QString& objectName);
    FolderForm* getActiveFolderForm();

    Ui::DoublePanelForm *ui;
};

#endif // DOUBLEPANELFORM_H
