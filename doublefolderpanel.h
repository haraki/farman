﻿#ifndef DOUBLEFOLDERPANEL_H
#define DOUBLEFOLDERPANEL_H

#include <QWidget>
#include <qdir.h>
#include "types.h"

namespace Ui
{
class DoubleFolderPanel;
}
class QString;

namespace Farman
{
class FolderForm;

class DoubleFolderPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleFolderPanel(ViewMode viewMode,
                               QString& l_path, QDir::Filters l_filterFlags, QDir::SortFlags l_sortFlags,
                               QString& r_path, QDir::Filters r_filterFlags, QDir::SortFlags r_sortFlags,
                               QWidget* parent = Q_NULLPTR);
    ~DoubleFolderPanel();
    void changeViewMode(ViewMode viewMode);

Q_SIGNALS:
    void currentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);

protected slots:
    void onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onFocusChanged(QWidget* oldWidget, QWidget* nowWidget);

    void emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void setActiveFolderForm(const QString& objectName);
    FolderForm* getActiveFolderForm();

    Ui::DoubleFolderPanel *ui;

    ViewMode m_viewMode;
};

}           // namespace Farman

#endif // DOUBLEFOLDERPANEL_H
