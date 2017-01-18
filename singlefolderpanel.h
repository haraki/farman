#ifndef SINGLEFOLDERPANEL_H
#define SINGLEFOLDERPANEL_H

#include <QWidget>
#include <qdir.h>
#include "folderpanelbase.h"

namespace Ui {
class SingleFolderPanel;
}
class QString;
class FolderForm;

class SingleFolderPanel : public FolderPanelBase
{
    Q_OBJECT

public:
    explicit SingleFolderPanel(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent/* = Q_NULLPTR*/);
    ~SingleFolderPanel();

private:
    FolderForm* getFolderForm() Q_DECL_OVERRIDE;

    Ui::SingleFolderPanel *ui;
};

#endif // SINGLEFOLDERPANEL_H
