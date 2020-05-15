#ifndef DOUBLEFOLDERPANEL_H
#define DOUBLEFOLDERPANEL_H

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
    ~DoubleFolderPanel() Q_DECL_OVERRIDE;

    void closeEvent(QCloseEvent* e) Q_DECL_OVERRIDE;

    void updateSettings();
    void updateFolderViewColorsSettings();

    void refresh();

    void setVisible(bool visible) Q_DECL_OVERRIDE;

    FolderForm* getFolderForm(PaneType pane);
    FolderForm* getActiveFolderForm();
    FolderForm* getInactiveFolderForm();

    void onSelectStorageBookmark();
    void onSetPaneMode(PaneMode paneMode);
    void onChangeSortSettings();
    void onChangeFilterSettings();
    void onCopy();
    void onMove();
    void onRemove();
    void onMakeDirectory();
    void onCreateNewFile();
    void onRename();
    void onAttributes();
    void onSelectAll();
    void onDeselectAll();
    void onCopyFullPath();
    void onCopyFileName();
    void onSetSameFolderToTarget();
    void onSetSameFolderFromTarget();

Q_SIGNALS:
    void outputConsole(const QString& consoleString);
    void statusChanged(const QString& statusString);
    void rootPathChanged(PaneType pane, const QString& path);
    void directoryBookmarked(PaneType pane, const QString &path, bool marked);
    void focusChanged(PaneType pane, bool inFocus);

    void openFile(const QString& path, ViewerType viewerType = ViewerType::Auto);
    void openWithApp(const QString& path);
    void openWithTextEditor(const QString& dirPath, const QStringList& fileNames);

    void copyFile(const QStringList& srcPaths, const QString& dstPath);
    void moveFile(const QStringList& srcPaths, const QString& dstPath);
    void removeFile(const QStringList& paths);
    void makeDirectory(const QString& path, const QString& dirName);
    void createNewFile(const QString& path, const QString& fileName);
    void renameFile(const QString& path, const QString& oldName, const QString& newName);
    void changeFileAttributes(const QString& path,
#ifdef Q_OS_WIN
                              const WinFileAttrFlags& newFileAttrFlags,
#else
                              const QFile::Permissions& newPermissions,
#endif
                              const QDateTime& newCreated,
                              const QDateTime& newLastModified);

protected Q_SLOTS:
    void onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onLeftFocusChanged(bool inFocus);
    void onRightFocusChanged(bool inFocus);
    void onLeftRootPathChanged(const QString& path);
    void onRightRootPathChanged(const QString& path);
    void onLeftDirectoryBookmarked(const QString &path, bool marked);
    void onRightDirectoryBookmarked(const QString &path, bool marked);

    void onOpenFile(const QString& path, ViewerType viewerType = ViewerType::Auto);
    void onOpenWithApp(const QString& path);

    void onCopyFile(const QStringList& srcPaths, const QString& dstPath);
    void onMoveFile(const QStringList& srcPaths, const QString& dstPath);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

    void emitOutputConsole(const QString& consoleString);
    void emitStatusChanged(const QString& statusString);
    void emitFocusChanged(PaneType pane, bool inFocus);
    void emitRootPathChanged(PaneType pane, const QString& path);
    void emitDirectoryBookmarked(PaneType pane, const QString &path, bool marked);

    void emitOpenFile(const QString& path, ViewerType viewerType = ViewerType::Auto);
    void emitOpenWithApp(const QString& path);

    void emitCopyFile(const QStringList& srcPaths, const QString& dstPath);
    void emitMoveFile(const QStringList& srcPaths, const QString& dstPath);
    void emitRemoveFile(const QStringList& paths);
    void emitMakeDirectory(const QString& path, const QString& dirName);
    void emitCreateNewFile(const QString& path, const QString& fileName);
    void emitRenameFile(const QString& path, const QString& oldName, const QString& newName);
    void emitChangeFileAttributes(const QString& path,
#ifdef Q_OS_WIN
                                  const WinFileAttrFlags& newFileAttrFlags,
#else
                                  const QFile::Permissions& newPermissions,
#endif
                                  const QDateTime& newCreated,
                                  const QDateTime& newLastModified);

    void setPaneMode(PaneMode paneMode);
    void setActivePane(PaneType pane, bool focus = true);

    Ui::DoubleFolderPanel *ui;

    PaneMode m_paneMode;
    PaneType m_activePane;
};

}           // namespace Farman

#endif // DOUBLEFOLDERPANEL_H
