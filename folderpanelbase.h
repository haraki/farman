#ifndef FOLDERPANELBASE_H
#define FOLDERPANELBASE_H

#include <QWidget>
#include <qdir.h>

class FolderForm;

class FolderPanelBase : public QWidget
{
public:
    FolderPanelBase(QWidget *parent = Q_NULLPTR);
    virtual ~FolderPanelBase();

    QString getPath();
    QDir::Filters getFilterFlags();
    QDir::SortFlags getSortFlags();

protected:
    virtual FolderForm* getFolderForm() = 0;
};

#endif // FOLDERPANELBASE_H
