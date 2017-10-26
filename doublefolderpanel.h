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
    explicit DoubleFolderPanel(QWidget* parent = Q_NULLPTR);
    ~DoubleFolderPanel();

    void closeEvent(QCloseEvent* e) Q_DECL_OVERRIDE;

    FolderForm* getActiveFolderForm();
    FolderForm* getInactiveFolderForm();
    FolderForm* getLeftFolderForm();
    FolderForm* getRightFolderForm();

    void onOpenInApp();
    void onSetViewMode(ViewMode viewMode);
    void onChangeSortSettings();
    void onChangeFilterSettings();
    void onCopy();
    void onMove();
    void onRemove();
    void onMakeDirectory();
    void onRename();

Q_SIGNALS:
    void statusChanged(const QString& statusString);
    void outputConsole(const QString& consoleString);

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
    void makeDirectory(const QString& path);
    void renameFile(const QString& path, const QString& name);

    void refresh();

    void emitOutputConsole(const QString& consoleString);

    Ui::DoubleFolderPanel *ui;

    ViewMode m_viewMode;
};

}           // namespace Farman

#endif // DOUBLEFOLDERPANEL_H
