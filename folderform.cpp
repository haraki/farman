#include <QDebug>
#include <QKeyEvent>
#include <QHeaderView>
#include <QFileDialog>
#include <QItemSelectionModel>
#include "folderform.h"
#include "ui_folderform.h"
#include "foldermodel.h"
#include "settings.h"

namespace Farman
{

FolderForm::FolderForm(FilterFlags filterFlags,
                       const QStringList& nameMaskFilters,
                       SectionType sortSectionType,
                       SortDirsType sortDirsType,
                       bool sortDotFirst,
                       Qt::CaseSensitivity sortCaseSensitivity,
                       Qt::SortOrder sortOrder,
                       QWidget *parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::FolderForm)
    , m_folderModel(new FolderModel(this))
{
    ui->setupUi(this);

    m_folderModel->setReadOnly(true);
    m_folderModel->setDynamicSortFilter(true);
    m_folderModel->setSortLocaleAware(true);

    setFilterFlags(filterFlags);
    setNameMaskFilters(nameMaskFilters);
    setSortSettings(sortSectionType, sortDirsType, sortDotFirst, sortCaseSensitivity, sortOrder);

    ui->folderView->setModel(m_folderModel);

    ui->folderView->setSelectionModel(m_folderModel->getSelectionModel());

    connect(m_folderModel->getSelectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
    connect(m_folderModel,
            SIGNAL(directoryLoaded(const QString&)),
            this,
            SLOT(onDirectoryLoaded(const QString&)));
    connect(m_folderModel,
            SIGNAL(layoutChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)),
            this,
            SLOT(onLayoutChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)));
    connect(m_folderModel,
            SIGNAL(layoutAboutToBeChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)),
            this,
            SLOT(onLayoutAboutToBeChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)));

    ui->folderView->installEventFilter(this);
}

FolderForm::~FolderForm()
{
    delete m_folderModel;
    delete ui;
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
                               const QFont& pathFont,
                               const QMap<ColorRoleType, QColor>& colors,
                               const QColor& pathColor,
                               const QColor& pathBgColor)
{
    m_folderModel->setFont(viewFont);

    ui->folderPathEdit->setFont(pathFont);

    m_folderModel->initBrushes(colors);

    QPalette pal;

    pal = ui->folderView->palette();
    pal.setColor(QPalette::Base, colors[ColorRoleType::Background]);
    ui->folderView->setAutoFillBackground(true);
    ui->folderView->setPalette(pal);

    pal = ui->folderPathEdit->palette();
    pal.setColor(QPalette::Text, pathColor);
    pal.setColor(QPalette::Base, pathBgColor);
    ui->folderPathEdit->setAutoFillBackground(true);
    ui->folderPathEdit->setPalette(pal);
}

void FolderForm::setFilterFlags(FilterFlags filterFlags)
{
    m_folderModel->setFilterFlags(filterFlags);

    QString flagsStr = tr("Hidden : ") + ((filterFlags & FilterFlag::Hidden) ? tr("Show") : tr("Hide"))
#ifdef Q_OS_WIN
                     + tr(", System : ") + ((filterFlags & FilterFlag::System) ? tr("Show") : tr("Hide"))
#endif
                     ;

    ui->filterFlagsLabel->setText(flagsStr);
}

FilterFlags FolderForm::getFilterFlags() const
{
    return m_folderModel->getFilterFlags();
}

void FolderForm::setNameMaskFilters(const QStringList& nameMaskFilters)
{
    m_folderModel->setNameFilters(nameMaskFilters);

    ui->filterNameMaskLabel->setText(tr("Filter : ") + nameMaskFilters.join(' '));
}

QStringList FolderForm::getNameMaskFilters() const
{
    return m_folderModel->nameFilters();
}

void FolderForm::setSortSettings(SectionType sectionType,
                                 SortDirsType dirsType,
                                 bool dotFirst,
                                 Qt::CaseSensitivity caseSensitivity,
                                 Qt::SortOrder order)
{
    m_folderModel->setSortSectionType(sectionType);
    m_folderModel->setSortDirsType(dirsType);
    m_folderModel->setSortDotFirst(dotFirst);
    m_folderModel->setSortCaseSensitivity(caseSensitivity);
    m_folderModel->setSortOrder(order);
}

SectionType FolderForm::getSortSectionType() const
{
    return m_folderModel->sortSectionType();
}

int FolderForm::getSortColumn() const
{
    return m_folderModel->sortColumn();
}

SortDirsType FolderForm::getSortDirsType() const
{
    return m_folderModel->sortDirsType();
}

bool FolderForm::getSortDotFirst() const
{
    return m_folderModel->sortDotFirst();
}

Qt::CaseSensitivity FolderForm::getSortCaseSensitivity() const
{
    return m_folderModel->sortCaseSensitivity();
}

Qt::SortOrder FolderForm::getSortOrder() const
{
    return m_folderModel->sortOrder();
}

int FolderForm::setPath(const QString& dirPath)
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

    m_folderModel->setRootPath(dirPath);

    ui->folderPathEdit->setText(dirPath);

    return 0;
}

QString FolderForm::getCurrentDirPath()
{
    QModelIndex index = ui->folderView->rootIndex();

    if(index.isValid())
    {
        return m_folderModel->filePath(index);
    }

    return "";
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

QList<QFileInfo> FolderForm::getSelectedFileInfoList()
{
    QModelIndexList indexList = m_folderModel->getSelectedIndexList();
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

void FolderForm::onDirectoryLoaded(const QString& path)
{
    qDebug() << "directory loaded." << path;

    QModelIndex currentRootIndex = ui->folderView->rootIndex();
    QModelIndex newRootIndex = m_folderModel->index(path);

    ui->folderView->setRootIndex(newRootIndex);

    if(currentRootIndex.parent() == newRootIndex)
    {
        ui->folderView->setCursor(currentRootIndex);
    }

    if(Settings::getInstance()->searchBookmarkDirPath(path) >= 0)
    {
        ui->bookmarkToolButton->setChecked(true);
    }
    else
    {
        ui->bookmarkToolButton->setChecked(false);
    }
}

void FolderForm::onLayoutChanged(const QList<QPersistentModelIndex> &parents/* = QList<QPersistentModelIndex>()*/, QAbstractItemModel::LayoutChangeHint hint/* = QAbstractItemModel::NoLayoutChangeHint*/)
{
    qDebug() << "FolderForm::onLayoutChanged() parents : " << parents << ", hint : " << hint;

    QModelIndex cursorIndex = ui->folderView->currentIndex();

    qDebug() << "cursorIndex.isValid() : " << cursorIndex.isValid() << ", cursorIndex.row() : " << cursorIndex.row();

    if(!cursorIndex.isValid() || cursorIndex.row() < 0)
    {
        // 初期カーソル位置はリストの先頭
        cursorIndex = m_folderModel->index(0, 0, ui->folderView->rootIndex());
        ui->folderView->setCursor(cursorIndex);
    }

    qDebug() << "rowCount : " << m_folderModel->rowCount(ui->folderView->rootIndex());
}

void FolderForm::onLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &parents/* = QList<QPersistentModelIndex>()*/, QAbstractItemModel::LayoutChangeHint hint/* = QAbstractItemModel::NoLayoutChangeHint*/)
{
    Q_UNUSED(parents)
    Q_UNUSED(hint)

    qDebug() << "FolderForm::onLayoutAboutToBeChanged()";
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
    const QModelIndex currentDirIndex = ui->folderView->rootIndex();
    const QString currentPath = m_folderModel->filePath(currentDirIndex);

    if(QDir(currentPath).isRoot())
    {
        return -1;
    }

    const QString newPath = m_folderModel->filePath(currentDirIndex.parent());

    qDebug() << "================== onGoToParentDir() : " << newPath;

    return setPath(newPath);
}

int FolderForm::onSelectDir()
{
    const QString currentPath = m_folderModel->filePath(ui->folderView->rootIndex());

    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Select folder."),
                                                        currentPath,
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(dirPath.isEmpty())
    {
        return -1;
    }

    return setPath(dirPath);
}

int FolderForm::onBookmarkDir(bool marked)
{
    const QModelIndex currentDirIndex = ui->folderView->rootIndex();
    const QString currentPath = m_folderModel->filePath(currentDirIndex);
    int index = Settings::getInstance()->searchBookmarkDirPath(currentPath);

    if(marked)
    {
        if(index < 0)
        {
           Settings::getInstance()->insertBookmarkDirPath(currentPath);
        }
    }
    else
    {
        if(index >= 0)
        {
           Settings::getInstance()->removeBookmarkDirPath(index);
        }
    }

    return 0;
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

void FolderForm::on_selectFolderButton_clicked()
{
    qDebug() << "FolderForm::on_selectFolderButton_clicked()";

    onSelectDir();
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

void FolderForm::emitFocusChanged(bool inFocus)
{
    emit focusChanged(inFocus);
}

}           // namespace Farman
