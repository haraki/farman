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
    ~DoubleFolderPanel();

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

protected Q_SLOTS:
    void onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onLeftFocusChanged(bool inFocus);
    void onRightFocusChanged(bool inFocus);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void setActiveFolderForm(const QString& objectName);

    void emitStatusChanged(const QString& statusString);

    Ui::DoubleFolderPanel *ui;

    PaneMode m_paneMode;

    QString m_activeFolderFormName;
};

}           // namespace Farman

#endif // DOUBLEFOLDERPANEL_H
