#ifndef FOLDERVIEW_H
#define FOLDERVIEW_H

#include <QTableView>

class FolderView : public QTableView
{
public:
    FolderView(QWidget *parent = Q_NULLPTR);
    virtual ~FolderView();
};

#endif // FOLDERVIEW_H
