#ifndef SINGLEFOLDERPANEL_H
#define SINGLEFOLDERPANEL_H

#include <QWidget>
#include <qdir.h>

namespace Ui {
class SingleFolderPanel;
}

class SingleFolderPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SingleFolderPanel(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent/* = Q_NULLPTR*/);
    ~SingleFolderPanel();

private:
    Ui::SingleFolderPanel *ui;
};

#endif // SINGLEFOLDERPANEL_H
