#ifndef DOUBLEFOLDERPANEL_H
#define DOUBLEFOLDERPANEL_H

#include <QWidget>
#include <qdir.h>

namespace Ui {
class DoubleFolderPanel;
}
class QString;
class FolderForm;

class DoubleFolderPanel : public QWidget
{
    Q_OBJECT

public:
    enum class ViewMode : int
    {
        Single = 0,
        Double = 1,

        ViewModeNum
    };

    explicit DoubleFolderPanel(ViewMode viewMode, QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = Q_NULLPTR);
    ~DoubleFolderPanel();
    void changeViewMode(ViewMode viewMode);

    QString getPath();
    QDir::Filters getFilterFlags();
    QDir::SortFlags getSortFlags();

Q_SIGNALS:
    void currentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);

protected slots:
    void onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);

    void emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void setActiveFolderForm(const QString& objectName);
    FolderForm* getActiveFolderForm();
    FolderForm* getFolderForm();

    Ui::DoubleFolderPanel *ui;

    ViewMode m_viewMode;
};

#endif // DOUBLEFOLDERPANEL_H
