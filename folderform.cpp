#include <QDebug>
#include <QKeyEvent>
#include <QHeaderView>
#include <QFileDialog>
#include <QItemSelectionModel>
#include "folderform.h"
#include "ui_folderform.h"
#include "foldermodel.h"
#include "default_settings.h"
#include "bookmarkmanager.h"
#include "bookmarkeditdialog.h"
#include "settings.h"

namespace Farman
{

FolderForm::FolderForm(PaneType pane,
                       FilterFlags filterFlags,
                       SectionType sortSectionType,
                       SectionType sortSectionType2nd,
                       SortDirsType sortDirsType,
                       bool sortDotFirst,
                       SortCaseSensitivity sortCaseSensitivity,
                       SortOrderType sortOrder,
                       FileSizeFormatType fileSizeformatType,
                       bool fileSizeComma,
                       DateFormatType dateFormatType,
                       QString dateOrgString,
                       QWidget *parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::FolderForm)
    , m_paneType(pane)
    , m_folderModel(new FolderModel(this))
    , m_historyManager(new HistoryManager(this))
{
    ui->setupUi(this);

    ui->goToFolderToolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogStart));

    m_folderModel->setFileSizeFormatType(fileSizeformatType);
    m_folderModel->setFileSizeComma(fileSizeComma);
    m_folderModel->setDateFormatType(dateFormatType);
    m_folderModel->setDateFormatOriginalString(dateOrgString);

    m_folderModel->setFilterFlags(filterFlags);
    m_folderModel->setNameFilters(QString(DEFAULT_NAME_MASK_FILTERS).simplified().split(' ', QString::SkipEmptyParts));

    setSortSettings(sortSectionType, sortSectionType2nd, sortDirsType, sortDotFirst, sortCaseSensitivity, sortOrder);

    ui->folderView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->folderView->verticalHeader()->setMinimumSectionSize(0);

    ui->folderView->setModel(m_folderModel);

    ui->folderView->setSelectionModel(m_folderModel->selectionModel());

    connect(ui->folderView,
            SIGNAL(selectedFile(const QString&, QItemSelectionModel::SelectionFlag)),
            this,
            SLOT(onSelectedFile(const QString&, QItemSelectionModel::SelectionFlag)));
    connect(m_folderModel->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
    connect(m_folderModel,
            SIGNAL(rootPathChanged(const QString&)),
            this,
            SLOT(onRootPathChanged(const QString&)));

    ui->folderView->installEventFilter(this);

    m_historyManager->initialize();
    m_historyManager->setList(Settings::getInstance()->getHistoryList(m_paneType));
}

FolderForm::~FolderForm()
{
    delete m_historyManager;
    delete m_folderModel;
    delete ui;
}

void FolderForm::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event)

    FolderAtStartup folderAtStartup = Settings::getInstance()->getFolderAtStartup(m_paneType);
    if(folderAtStartup == FolderAtStartup::LastTime)
    {
        Settings::getInstance()->setFolderPath(m_paneType, getCurrentDirPath());
    }

    Settings::getInstance()->setHistoryList(m_paneType, m_historyManager->getList());
}

bool FolderForm::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type())
    {
    case QEvent::FocusIn:
        emitFocusChanged(true);

        break;

    case QEvent::FocusOut:
        emitFocusChanged(false);
        break;

    default:
        break;
    }

    return ret;
}

void FolderForm::setAppearance(const QFont& viewFont,
                               qreal viewRowHeight,
                               const QFont& pathFont,
                               const QColor& pathColor,
                               const QColor& pathBgColor,
                               int cursorWidth,
                               const QColor cursorActiveColor,
                               const QColor cursorInactiveColor,
                               bool loopMove,
                               bool moveOpenViewer,
                               DragAndDropBehaviorType behaviorType)
{
    m_folderModel->setFont(viewFont);

    ui->folderView->setCursorAppearance(cursorWidth, cursorActiveColor, cursorInactiveColor);
    ui->folderView->setCursorBehaivior(loopMove, moveOpenViewer);
    ui->folderView->setDragAndDropBehavior(behaviorType);

    ui->folderView->verticalHeader()->setDefaultSectionSize(static_cast<int>(QFontMetrics(viewFont).height() * viewRowHeight));

    ui->folderPathEdit->setFont(pathFont);

    QPalette pal = ui->folderPathEdit->palette();
    pal.setColor(QPalette::Text, pathColor);
    pal.setColor(QPalette::Base, pathBgColor);
    ui->folderPathEdit->setAutoFillBackground(true);
    ui->folderPathEdit->setPalette(pal);
}

void FolderForm::setAppearanceFolderViewColors(const QMap<ColorRoleType, QColor>& folderViewColors, bool folderColorTopPrio)
{
    m_folderModel->initBrushes(folderViewColors, folderColorTopPrio);

    QPalette pal = ui->folderView->palette();
    pal.setColor(QPalette::Base, folderViewColors[ColorRoleType::Background]);
    ui->folderView->setAutoFillBackground(true);
    ui->folderView->setPalette(pal);
}

void FolderForm::setFileSizeFormatType(FileSizeFormatType formatType)
{
    m_folderModel->setFileSizeFormatType(formatType);
}

FileSizeFormatType FolderForm::getFileSizeFormatType() const
{
    return m_folderModel->fileSizeFormatType();
}

void FolderForm::setFileSizeComma(bool fileSizeComma)
{
    m_folderModel->setFileSizeComma(fileSizeComma);
}

bool FolderForm::getFileSizeComma() const
{
    return m_folderModel->fileSizeComma();
}

void FolderForm::setDateFormatType(DateFormatType formatType)
{
    m_folderModel->setDateFormatType(formatType);
}

DateFormatType FolderForm::getDateFormatType() const
{
    return m_folderModel->dateFormatType();
}

void FolderForm::setDateFormatOriginalString(const QString& orgString)
{
    m_folderModel->setDateFormatOriginalString(orgString);
}

QString FolderForm::getDateFormatOriginalString() const
{
    return m_folderModel->dateFormatOriginalString();
}

void FolderForm::setFilterFlags(FilterFlags filterFlags)
{
    m_folderModel->setFilterFlags(filterFlags);
}

FilterFlags FolderForm::getFilterFlags() const
{
    return m_folderModel->filterFlags();
}

void FolderForm::setNameMaskFilters(const QStringList& nameMaskFilters)
{
    m_folderModel->setNameFilters(nameMaskFilters);
}

QStringList FolderForm::getNameMaskFilters() const
{
    return m_folderModel->nameFilters();
}

void FolderForm::updateFilterLabel()
{
    qDebug() << "FolderForm::updateFilterLabel()";

    FilterFlags filterFlags = getFilterFlags();

    QString filterStr = tr("Hidden : ") + ((filterFlags & FilterFlag::Hidden) ? tr("Show") : tr("Hide"))
#ifdef Q_OS_WIN
                      + ", " + tr("System : ") + ((filterFlags & FilterFlag::System) ? tr("Show") : tr("Hide"))
#endif
                      ;

    if(filterFlags & FilterFlag::Files && !(filterFlags & FilterFlag::Dirs))
    {
        filterStr += ", " + tr("File only");
    }
    else if(filterFlags & FilterFlag::Dirs && !(filterFlags & FilterFlag::Files))
    {
        filterStr += ", " + tr("Folder only");
    }

    ui->filterLabel->setText(tr("Filter : ") + getNameMaskFilters().join(' ') + ", " + filterStr);
}

void FolderForm::setSortSettings(SectionType sectionType,
                                 SectionType sectionType2nd,
                                 SortDirsType dirsType,
                                 bool dotFirst,
                                 SortCaseSensitivity caseSensitivity,
                                 SortOrderType order)
{
    m_folderModel->setSortSectionType(sectionType);
    m_folderModel->setSortSectionType2nd(sectionType2nd);
    m_folderModel->setSortDirsType(dirsType);
    m_folderModel->setSortDotFirst(dotFirst);
    m_folderModel->setSortCaseSensitivity(caseSensitivity);
    m_folderModel->setSortOrder(order);
}

SectionType FolderForm::getSortSectionType() const
{
    return m_folderModel->sortSectionType();
}

SectionType FolderForm::getSortSectionType2nd() const
{
    return m_folderModel->sortSectionType2nd();
}

#if 0
int FolderForm::getSortColumn() const
{
    return m_folderModel->sortColumn();
}
#endif

SortDirsType FolderForm::getSortDirsType() const
{
    return m_folderModel->sortDirsType();
}

bool FolderForm::getSortDotFirst() const
{
    return m_folderModel->sortDotFirst();
}

SortCaseSensitivity FolderForm::getSortCaseSensitivity() const
{
    return m_folderModel->sortCaseSensitivity();
}

SortOrderType FolderForm::getSortOrder() const
{
    return m_folderModel->sortOrder();
}

int FolderForm::setPath(const QString& dirPath, bool addHistory/* = true*/)
{
    if(m_folderModel->rootPath() == dirPath)
    {
        return 0;
    }

    // ディレクトリが空(".." も存在しない)場合は Open できないとみなしてエラー
    if(QDir(dirPath).isEmpty(QDir::AllEntries | QDir::NoDot))
    {
        qDebug() << dirPath << " size() == 0";

        return -1;
    }

    m_folderModel->clearSelected();
    setFilterFlags(getFilterFlags() | FilterFlag::AllEntrys);
    setNameMaskFilters(QString(DEFAULT_NAME_MASK_FILTERS).simplified().split(' ', QString::SkipEmptyParts));
    updateFilterLabel();

    int ret = m_folderModel->setRootPath(dirPath);
    if(ret < 0)
    {
        return -1;
    }

    ui->folderPathEdit->setText(dirPath);

    if(addHistory && m_historyManager->search(dirPath) != 0)
    {
        m_historyManager->setPath(dirPath);
    }

    return 0;
}

QString FolderForm::getCurrentDirPath()
{
    return m_folderModel->rootPath();
}

QString FolderForm::getCurrentFileName()
{
    QModelIndex index = ui->folderView->currentIndex();

    if(index.isValid())
    {
        return m_folderModel->fileName(index);
    }

    return "";
}

QFileInfo FolderForm::getCurrentFileInfo()
{
    QModelIndex index = ui->folderView->currentIndex();

    if(index.isValid())
    {
        return m_folderModel->fileInfo(index);
    }

    return QFileInfo();
}

QString FolderForm::getPreviousDirPath()
{
    Q_ASSERT(m_historyManager != Q_NULLPTR);

    return m_historyManager->getPrevious();
}

QString FolderForm::getNextDirPath()
{
    Q_ASSERT(m_historyManager != Q_NULLPTR);

    return m_historyManager->getNext();
}

const HistoryManager* FolderForm::getHistoryManager()
{
    return m_historyManager;
}

QList<QFileInfo> FolderForm::getSelectedFileInfoList()
{
    QModelIndexList indexList = m_folderModel->selectedIndexList();
    if(indexList.size() == 0 && getCurrentFileName() != "" && getCurrentFileName() != "..")
    {
        // 選択しているアイテムが無くても、".." 以外の箇所にカーソルがあれば、それを選択しているものとする
        indexList.push_back(ui->folderView->currentIndex());
    }

    QList<QFileInfo> selectedFileInfoList;
    for(auto modelIndex : indexList)
    {
        selectedFileInfoList.push_back(m_folderModel->fileInfo(modelIndex).absoluteFilePath());
    }

    return selectedFileInfoList;
}

void FolderForm::onSelectedFile(const QString& path, QItemSelectionModel::SelectionFlag selectionFlag)
{
    qDebug() << "FolderForm::onSelectedFile : path : " << path << ", selectionFlag : " << selectionFlag;

    updateMarkedLabel();
}

void FolderForm::onCurrentChanged(const QModelIndex& newIndex, const QModelIndex& oldIndex)
{
    qDebug() << "FolderForm::onCurrentChanged : old : " << oldIndex.row() << " new : " << newIndex.row();

    QModelIndex topLeft, bottomRight;
    if(newIndex.row() < oldIndex.row())
    {
        topLeft = m_folderModel->index(newIndex.row(), 0);
        bottomRight = m_folderModel->index(oldIndex.row(), m_folderModel->columnCount());
    }
    else
    {
        topLeft = m_folderModel->index(oldIndex.row(), 0);
        bottomRight = m_folderModel->index(newIndex.row(), m_folderModel->columnCount());
    }

    // カーソルが移動した箇所を再描画する
    emit ui->folderView->refresh(topLeft, bottomRight);

    emitCurrentChanged((newIndex.row() >= 0) ? m_folderModel->fileInfo(newIndex) : QFileInfo(),
                       (oldIndex.row() >= 0) ? m_folderModel->fileInfo(oldIndex) : QFileInfo());
}

void FolderForm::onRootPathChanged(const QString& path)
{
//    qDebug() << "directory loaded." << path;

//    QModelIndex currentRootIndex = ui->folderView->rootIndex();
//    QModelIndex newRootIndex = m_folderModel->index(path);

//    qDebug() << "FolderForm::onRootPathChanged : current : " << currentRootIndex.row() << " new : " << newRootIndex.row();

//    ui->folderView->setRootIndex(newRootIndex);

//    if(currentRootIndex.parent() == newRootIndex)
//    {
//        ui->folderView->setCursor(currentRootIndex);
//    }
//    else
    {
        // 初期カーソル位置はリストの先頭
        QModelIndex cursorIndex = m_folderModel->index(0, 0);
        ui->folderView->setCursor(cursorIndex);
    }

    updateMarkedLabel();
    checkBookmark();

    emitRootPathChanged(path);
}

void FolderForm::setCursor(const QString& fileName)
{
    const QString currentPath = getCurrentDirPath();
    QModelIndex newIndex = m_folderModel->index(QFileInfo(currentPath, fileName).absoluteFilePath());

    ui->folderView->setCursor(newIndex);
}

int FolderForm::onGoToChildDir()
{
    const QModelIndex currentIndex = ui->folderView->currentIndex();

    if(!m_folderModel->isDir(currentIndex))
    {
        return -1;
    }

    const QString newPath = m_folderModel->filePath(currentIndex);

    qDebug() << "================== onGoToChildDir() : " << newPath;

    return setPath(newPath);
}

int FolderForm::onGoToParentDir()
{
//    const QModelIndex currentDirIndex = ui->folderView->rootIndex();
    const QString currentPath = m_folderModel->rootPath();
    QDir currentDir(currentPath);

    if(currentDir.isRoot() || !currentDir.cdUp())
    {
        return -1;
    }

    const QString newPath = currentDir.absolutePath();

    qDebug() << "================== onGoToParentDir() : " << newPath;

    return setPath(newPath);
}

int FolderForm::onPreviousDir()
{
    QString path = m_historyManager->getPrevious();
    if(path.isEmpty())
    {
        return -1;
    }

    m_historyManager->setPreviousIndex();

    return setPath(path, false);
}

int FolderForm::onNextDir()
{
    QString path = m_historyManager->getNext();
    if(path.isEmpty())
    {
        return -1;
    }

    m_historyManager->setNextIndex();

    return setPath(path, false);
}

int FolderForm::onChangeDir()
{
    const QString currentPath = m_folderModel->rootPath();

    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Change folder."),
                                                        currentPath,
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(dirPath.isEmpty())
    {
        return -1;
    }

    return setPath(dirPath);
}

void FolderForm::onBookmarkDir(bool marked)
{
    const QString currentPath = getCurrentDirPath();
    int index = BookmarkManager::getInstance()->search(currentPath);

    if(marked)
    {
        if(index < 0)
        {
            BookmarkInfo retInfo;
            if(BookmarkEditDialog::launchDialog(currentPath,
                                                {BookmarkInfo::Type::User,
                                                 currentPath,
                                                 currentPath},
                                                retInfo,
                                                this) != QDialog::Accepted)
            {
                return;
            }

            BookmarkManager::getInstance()->insert({retInfo.getName(), retInfo.getPath()});
        }
    }
    else
    {
        if(index >= 0)
        {
            BookmarkManager::getInstance()->remove(index);
        }
    }

    emitDirectoryBookmarked(currentPath, marked);
}

void FolderForm::onSelectAll()
{
    qDebug() << "FolderForm::onSelectAll()";

    m_folderModel->setSelectAll();

    updateMarkedLabel();            // setSelectAll() は onSelectedFile は通知されないので、自分で updateMarkedLabel() を呼ぶ
}

void FolderForm::onDeselectAll()
{
    qDebug() << "FolderForm::onDeselectAll()";

    QModelIndex currentIndex = ui->folderView->currentIndex();      // clearSelected() するとカーソルが消えてしまうので、再設定のために保存

    m_folderModel->clearSelected();

    ui->folderView->setCursor(currentIndex);

    updateMarkedLabel();            // clearSelected() は onSelectedFile は通知されないので、自分で updateMarkedLabel() を呼ぶ
}

void FolderForm::checkBookmark()
{
    qDebug() << "FolderForm::checkBookmark()";

    const QString& currentPath = getCurrentDirPath();

    ui->bookmarkToolButton->blockSignals(true);
    if(BookmarkManager::getInstance()->search(currentPath) >= 0)
    {
        ui->bookmarkToolButton->setChecked(true);
    }
    else
    {
        ui->bookmarkToolButton->setChecked(false);
    }
    ui->bookmarkToolButton->blockSignals(false);
}

void FolderForm::updateMarkedLabel()
{
    qDebug() << "FolderForm::updateMarkedLabel()";

    qint64 size = 0;

    if(m_folderModel->selectedIndexList().size() > 0)
    {
        for(auto fileInfo : getSelectedFileInfoList())
        {
            if(!fileInfo.isDir())
            {
                size += fileInfo.size();
            }
        }
    }

    ui->markedLabel->setText(tr("Marked : %1 / %2, %L3 Bytes")
                             .arg(m_folderModel->selectedIndexList().size())
                             .arg(m_folderModel->fileDirNum())
                             .arg(size));
}

void FolderForm::refresh(bool clearSelected/* = false */)
{
    if(clearSelected)
    {
        m_folderModel->clearSelected();
    }

    m_folderModel->refresh();
}

int FolderForm::getTotalColumnWidth(int withOutColumn)
{
    int totalWidth = 0;

    for(int i = 0;i < ui->folderView->horizontalHeader()->count();i++)
    {
        if(i != withOutColumn)
        {
            totalWidth += ui->folderView->columnWidth(i);
        }
    }

    return totalWidth;
}

void FolderForm::on_goToFolderToolButton_clicked()
{
    qDebug() << "FolderForm::on_goToFolderToolButton_clicked()";

    onChangeDir();
}

void FolderForm::on_bookmarkToolButton_toggled(bool checked)
{
    qDebug() << "FolderForm::on_bookmarkToolButton_toggled() : " << checked;

    onBookmarkDir(checked);
}

void FolderForm::emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    emit currentChanged(newFileInfo, oldFileInfo);
}

void FolderForm::emitRootPathChanged(const QString &path)
{
    emit rootPathChanged(path);
}

void FolderForm::emitFocusChanged(bool inFocus)
{
    emit focusChanged(inFocus);
}

void FolderForm::emitDirectoryBookmarked(const QString &path, bool marked)
{
    emit directoryBookmarked(path, marked);
}

}           // namespace Farman
