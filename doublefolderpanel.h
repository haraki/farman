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

    void refresh();

    void setVisible(bool visible) Q_DECL_OVERRIDE;

    FolderForm* getActiveFolderForm();
    FolderForm* getInactiveFolderForm();
    FolderForm* getLeftFolderForm();
    FolderForm* getRightFolderForm();

    void onSetPaneMode(PaneMode paneMode);
    void onChangeSortSettings();
    void onChangeFilterSettings();
    void onCopy();
    void onMove();
    void onRemove();
    void onMakeDirectory();
    void onRename();
    void onAttributes();

Q_SIGNALS:
    void statusChanged(const QString& statusString);

    void openFile(const QString& path, ViewerType viewerType = ViewerType::Auto);
    void openWithApp(const QString& path);
    void openWithTextEditor(const QString& dirPath, const QStringList& fileNames);

    void copyFile(const QStringList& srcPaths, const QString& dstPath);
    void moveFile(const QStringList& srcPaths, const QString& dstPath);
    void removeFile(const QStringList& paths);
    void makeDirectory(const QString& path, const QString& dirName);
    void renameFile(const QString& path, const QString& oldName, const QString& newName);
    void changeFileAttributes(const QString& path,
                              const QFile::Permissions& newPermissions,
                              const QDateTime& newCreated,
                              const QDateTime& newLastModified);

protected Q_SLOTS:
    void onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onLeftFocusChanged(bool inFocus);
    void onRightFocusChanged(bool inFocus);

    void onOpenFile(const QString& path, ViewerType viewerType = ViewerType::Auto);
    void onOpenWithApp(const QString& path);
    void onOpenWithTextEditor(const QString& dirPath, const QStringList& fileNames);

    void onCopyFile(const QStringList& srcPaths, const QString& dstPath);
    void onMoveFile(const QStringList& srcPaths, const QString& dstPath);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

    void emitStatusChanged(const QString& statusString);

    void emitOpenFile(const QString& path, ViewerType viewerType = ViewerType::Auto);
    void emitOpenWithApp(const QString& path);
    void emitOpenWithTextEditor(const QString& dirPath, const QStringList& fileNames);

    void emitCopyFile(const QStringList& srcPaths, const QString& dstPath);
    void emitMoveFile(const QStringList& srcPaths, const QString& dstPath);
    void emitRemoveFile(const QStringList& paths);
    void emitMakeDirectory(const QString& path, const QString& dirName);
    void emitRenameFile(const QString& path, const QString& oldName, const QString& newName);
    void emitChangeFileAttributes(const QString& path,
                                  const QFile::Permissions& newPermissions,
                                  const QDateTime& newCreated,
                                  const QDateTime& newLastModified);

    void setPaneMode(PaneMode paneMode);
    void setActivePane(PaneType pane);

    Ui::DoubleFolderPanel *ui;

    PaneMode m_paneMode;
    PaneType m_activePane;
};

}           // namespace Farman

#endif // DOUBLEFOLDERPANEL_H
