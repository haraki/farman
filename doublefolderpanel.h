#ifndef DOUBLEFOLDERPANEL_H
#define DOUBLEFOLDERPANEL_H

#include <QWidget>
#include <qdir.h>
#include "folderpanelbase.h"

namespace Ui {
class DoubleFolderPanel;
}
class QString;
class FolderForm;

class DoubleFolderPanel : public FolderPanelBase
{
    Q_OBJECT

public:
    explicit DoubleFolderPanel(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = Q_NULLPTR);
    ~DoubleFolderPanel();

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void setActiveFolderForm(const QString& objectName);
    FolderForm* getActiveFolderForm();
    FolderForm* getFolderForm() Q_DECL_OVERRIDE;

    Ui::DoubleFolderPanel *ui;
};

#endif // DOUBLEFOLDERPANEL_H
