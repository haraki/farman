#ifndef DUALPANELFORM_H
#define DUALPANELFORM_H

#include <QWidget>
#include <qdir.h>

namespace Ui {
class DualPanelForm;
}
class QString;
class FolderForm;

class DualPanelForm : public QWidget
{
    Q_OBJECT

public:
    explicit DualPanelForm(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = Q_NULLPTR);
    ~DualPanelForm();

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void setActiveFolderForm(const QString& objectName);
    FolderForm* getActiveFolderForm();

    Ui::DualPanelForm *ui;
};

#endif // TWOCOLUMNFORM_H
