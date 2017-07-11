#ifndef DOUBLEFOLDERPANEL_H
#define DOUBLEFOLDERPANEL_H

#include <qdir.h>
#include <QWidget>
#include <QModelIndexList>
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
    FolderForm* getActiveFolderForm();
    FolderForm* getInactiveFolderForm();

    void onCopy();
    void onMove();
    void onRemove();

Q_SIGNALS:
    void statusChanged(const QString& statusString);

protected Q_SLOTS:
    void onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onLeftFocusChanged(bool inFocus);
    void onRightFocusChanged(bool inFocus);

    void onCopyFileFinished(int result);
    void onCopyFileError(const QString& err);
    void onMoveFileFinished(int result);
    void onMoveFileError(const QString& err);
    void onRemoveFileFinished(int result);
    void onRemoveFileError(const QString& err);

    void onConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, int methodType);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void emitStatusChanged(const QString& statusString);
    void setActiveFolderForm(const QString& objectName);
    void copyFile(const QStringList& srcPaths, const QString& dstPath);
    void moveFile(const QStringList& srcPaths, const QString& dstPath);
    void removeFile(const QStringList& paths);

    void refresh();

    Ui::DoubleFolderPanel *ui;

    ViewMode m_viewMode;
};

}           // namespace Farman

#endif // DOUBLEFOLDERPANEL_H
