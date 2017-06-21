#include <qdir.h>
#include <QKeyEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QMessageBox>
#include "doublefolderpanel.h"
#include "ui_doublefolderpanel.h"
#include "folderform.h"
#include "folderview.h"
#include "foldermodel.h"
#include "copyworker.h"
#include "removeworker.h"

namespace Farman
{

DoubleFolderPanel::DoubleFolderPanel(ViewMode viewMode,
                                     QString& l_path, QDir::Filters l_filterFlags, QDir::SortFlags l_sortFlags,
                                     QString& r_path, QDir::Filters r_filterFlags, QDir::SortFlags r_sortFlags,
                                     QWidget* parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::DoubleFolderPanel)
    , m_viewMode(viewMode)
{
    ui->setupUi(this);

    QVBoxLayout* l_vLayout = new QVBoxLayout();
    l_vLayout->setSpacing(6);
    l_vLayout->setObjectName(QStringLiteral("l_vLayout"));
    l_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* l_folderForm = new FolderForm(l_filterFlags, l_sortFlags, this);
    l_folderForm->setObjectName(QStringLiteral("l_folderForm"));
    l_folderForm->setPath(l_path);

    l_vLayout->addWidget(l_folderForm);

    ui->leftPanel->setLayout(l_vLayout);

    FolderView* l_folderView = l_folderForm->findChild<FolderView*>("folderView");
    if(l_folderView != Q_NULLPTR)
    {
        l_folderView->installEventFilter(this);
    }

    QVBoxLayout* r_vLayout = new QVBoxLayout();
    r_vLayout->setSpacing(6);
    r_vLayout->setObjectName(QStringLiteral("r_vLayout"));
    r_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* r_folderForm = new FolderForm(r_filterFlags, r_sortFlags, this);
    r_folderForm->setObjectName(QStringLiteral("r_folderForm"));
    r_folderForm->setPath(r_path);

    r_vLayout->addWidget(r_folderForm);

    ui->rightPanel->setLayout(r_vLayout);

    FolderView* r_folderView = r_folderForm->findChild<FolderView*>("folderView");
    if(r_folderView != Q_NULLPTR)
    {
        r_folderView->installEventFilter(this);
    }

    connect(l_folderForm,
            SIGNAL(currentChanged(QFileInfo,QFileInfo)),
            this,
            SLOT(onLeftCurrentChanged(QFileInfo,QFileInfo)));
    connect(r_folderForm,
            SIGNAL(currentChanged(QFileInfo,QFileInfo)),
            this,
            SLOT(onRightCurrentChanged(QFileInfo,QFileInfo)));
    connect(l_folderForm,
            SIGNAL(focusChanged(bool)),
            this,
            SLOT(onLeftFocusChanged(bool)));
    connect(r_folderForm,
            SIGNAL(focusChanged(bool)),
            this,
            SLOT(onRightFocusChanged(bool)));

    setActiveFolderForm("l_folderForm");

    changeViewMode(viewMode);
}

DoubleFolderPanel::~DoubleFolderPanel()
{
    delete ui;
}

void DoubleFolderPanel::changeViewMode(ViewMode viewMode)
{
    if(viewMode == m_viewMode)
    {
        return;
    }

    m_viewMode = viewMode;

    switch(viewMode)
    {
    case ViewMode::Single:
        if(getActiveFolderForm()->objectName() == "l_folderForm")
        {
            ui->leftPanel->setVisible(true);
            ui->rightPanel->setVisible(false);
        }
        else
        {
            ui->leftPanel->setVisible(false);
            ui->rightPanel->setVisible(true);
        }

        break;

    case ViewMode::Double:
        ui->leftPanel->setVisible(true);
        ui->rightPanel->setVisible(true);

        break;

    default:
        break;
    }
}

bool DoubleFolderPanel::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type()) {
    case QEvent::KeyPress:
    {
        Qt::Key key = static_cast<Qt::Key>(dynamic_cast<QKeyEvent*>(e)->key());

        qDebug() << "DoubleFolderPanel::eventFilter : " << key;

        FolderForm* activeForm = getActiveFolderForm();
        if(activeForm == Q_NULLPTR)
        {
            break;
        }

        switch(key)
        {
        case Qt::Key_Left:
            if(m_viewMode == ViewMode::Single || activeForm->objectName() == "l_folderForm")
            {
                activeForm->onGoToParent();
            }
            else
            {
                setActiveFolderForm("l_folderForm");
            }

            ret = true;

            break;

        case Qt::Key_Right:
            if(m_viewMode == ViewMode::Double)
            {
                if(activeForm->objectName() == "r_folderForm")
                {
                    activeForm->onGoToParent();
                }
                else
                {
                    setActiveFolderForm("r_folderForm");
                }
            }

            ret = true;

            break;

        default:
            break;
        }

        break;
    }
    default:
        break;
    }

    return ret;
}

void DoubleFolderPanel::onCopy()
{
    qDebug() << "DoubleFolderPanel::onCopy()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
        if(selectedFileInfoList.size() > 0)
        {
            FolderForm* inactiveForm = getInactiveFolderForm();
            if(inactiveForm != Q_NULLPTR)
            {
                QStringList srcPaths;

                for(QFileInfo fileInfo : selectedFileInfoList)
                {
                    srcPaths.push_back(fileInfo.absoluteFilePath());

                    qDebug() << fileInfo.absoluteFilePath();
                }

                fileCopy(srcPaths, inactiveForm->getCurrentDirPath());
            }
        }
    }
}

void DoubleFolderPanel::onMove()
{
    qDebug() << "DoubleFolderPanel::onMove()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
        if(selectedFileInfoList.size() > 0)
        {
            FolderForm* inactiveForm = getInactiveFolderForm();
            if(inactiveForm != Q_NULLPTR)
            {
                QStringList srcPaths;

                for(QFileInfo fileInfo : selectedFileInfoList)
                {
                    srcPaths.push_back(fileInfo.absoluteFilePath());

                    qDebug() << fileInfo.absoluteFilePath();
                }

                fileMove(srcPaths, inactiveForm->getCurrentDirPath());
            }
        }
    }
}

void DoubleFolderPanel::onRemove()
{
    qDebug() << "DoubleFolderPanel::onRemove()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
        if(selectedFileInfoList.size() > 0)
        {
            QStringList paths;

            for(QFileInfo fileInfo : selectedFileInfoList)
            {
                paths.push_back(fileInfo.absoluteFilePath());

                qDebug() << fileInfo.absoluteFilePath();
            }

            fileRemove(paths);
        }
    }
}

void DoubleFolderPanel::onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "DoubleFolderPanel::onLeftCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR && activeForm->objectName() == "l_folderForm")
    {
        emitStatusChanged(newFileInfo.absoluteFilePath());
    }
}

void DoubleFolderPanel::onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "DoubleFolderPanel::onRightCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR && activeForm->objectName() == "r_folderForm")
    {
        emitStatusChanged(newFileInfo.absoluteFilePath());
    }
}

void DoubleFolderPanel::onLeftFocusChanged(bool inFocus)
{
    qDebug() << "DoubleFolderPanel::onLeftFocusChanged : inFocus : " << inFocus;

    if(inFocus)
    {
        FolderForm* activeForm = getActiveFolderForm();
        if(activeForm != Q_NULLPTR && activeForm->objectName() == "l_folderForm")
        {
            emitStatusChanged(activeForm->getCurrentFileInfo().absoluteFilePath());
        }
    }
}

void DoubleFolderPanel::onRightFocusChanged(bool inFocus)
{
    qDebug() << "DoubleFolderPanel::onRightFocusChanged : inFocus : " << inFocus;

    if(inFocus)
    {
        FolderForm* activeForm = getActiveFolderForm();
        if(activeForm != Q_NULLPTR && activeForm->objectName() == "r_folderForm")
        {
            emitStatusChanged(activeForm->getCurrentFileInfo().absoluteFilePath());
        }
    }
}

void DoubleFolderPanel::emitStatusChanged(const QString& statusString)
{
    emit statusChanged(statusString);
}

void DoubleFolderPanel::setActiveFolderForm(const QString& objectName)
{
    FolderForm* folderForm = findChild<FolderForm*>(objectName);
    if(folderForm != Q_NULLPTR)
    {
        FolderView* folderView = folderForm->findChild<FolderView*>("folderView");
        if(folderView != Q_NULLPTR)
        {
            folderView->setFocus();
        }
    }
}

FolderForm* DoubleFolderPanel::getActiveFolderForm()
{
    QWidget* fw = focusWidget();
    if(fw == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }

    if(fw->objectName() != "folderView")
    {
        return Q_NULLPTR;
    }

    return dynamic_cast<FolderForm*>(fw->parent());
}

FolderForm* DoubleFolderPanel::getInactiveFolderForm()
{
    FolderForm* inactiveForm = Q_NULLPTR;
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        if(activeForm->objectName() == "r_folderForm")
        {
            inactiveForm = findChild<FolderForm*>("l_folderForm");
        }
        else
        {
            inactiveForm = findChild<FolderForm*>("r_folderForm");
        }
    }

    return inactiveForm;
}

void DoubleFolderPanel::fileCopy(const QStringList& srcPaths, const QString& dstPath)
{
    if(QMessageBox::question(this->parentWidget(), tr("Confirm"), tr("copy?")) == QMessageBox::Yes)
    {
        Worker* worker = new CopyWorker(srcPaths, dstPath, false, this);

        connect(worker,
                SIGNAL(finished(int)),
                this,
                SLOT(onFileCopyFinished(int)));
        connect(worker,
                SIGNAL(error(QString)),
                this,
                SLOT(onFileCopyError(QString)));

        worker->start();

        return;
    }
}

void DoubleFolderPanel::fileMove(const QStringList& srcPaths, const QString& dstPath)
{
    if(QMessageBox::question(this->parentWidget(), tr("Confirm"), tr("move?")) == QMessageBox::Yes)
    {
        Worker* worker = new CopyWorker(srcPaths, dstPath, true, this);

        connect(worker,
                SIGNAL(finished(int)),
                this,
                SLOT(onFileMoveFinished(int)));
        connect(worker,
                SIGNAL(error(QString)),
                this,
                SLOT(onFileMoveError(QString)));

        worker->start();

        return;
    }
}

void DoubleFolderPanel::fileRemove(const QStringList& paths)
{
    if(QMessageBox::question(this->parentWidget(), tr("Confirm"), tr("remove?")) == QMessageBox::Yes)
    {
        Worker* worker = new RemoveWorker(paths, this);

        connect(worker,
                SIGNAL(finished(int)),
                this,
                SLOT(onFileRemoveFinished(int)));
        connect(worker,
                SIGNAL(error(QString)),
                this,
                SLOT(onFileRemoveError(QString)));

        worker->start();

        return;
    }
}

void DoubleFolderPanel::onFileCopyFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onFileCopyFinished : result : " << result;

    FolderForm* inactiveFolderForm = getInactiveFolderForm();
    if(inactiveFolderForm != nullptr)
    {
        inactiveFolderForm->refresh();
    }
}

void DoubleFolderPanel::onFileCopyError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onFileCopyError : err : " << err;

}

void DoubleFolderPanel::onFileMoveFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onFileMoveFinished : result : " << result;

    FolderForm* activeFolderForm = getActiveFolderForm();
    if(activeFolderForm != nullptr)
    {
        activeFolderForm->refresh();
    }

    FolderForm* inactiveFolderForm = getInactiveFolderForm();
    if(inactiveFolderForm != nullptr)
    {
        inactiveFolderForm->refresh();
    }
}

void DoubleFolderPanel::onFileMoveError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onFileMoveError : err : " << err;

}

void DoubleFolderPanel::onFileRemoveFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onFileRemoveFinished : result : " << result;

    FolderForm* activeFolderForm = getActiveFolderForm();
    if(activeFolderForm != nullptr)
    {
        activeFolderForm->refresh();
    }
}

void DoubleFolderPanel::onFileRemoveError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onFileREmoveError : err : " << err;

}

}           // namespace Farman
