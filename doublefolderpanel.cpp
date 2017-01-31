﻿#include <QKeyEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <qdir.h>
#include "doublefolderpanel.h"
#include "ui_doublefolderpanel.h"
#include "folderform.h"
#include "folderview.h"
#include "foldermodel.h"

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
    connect(qApp,
            SIGNAL(focusChanged(QWidget*,QWidget*)),
            this,
            SLOT(onFocusChanged(QWidget*,QWidget*)));

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

void DoubleFolderPanel::onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "DoubleFolderPanel::onLeftCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR && activeForm->objectName() == "l_folderForm")
    {
        emitCurrentChanged(newFileInfo, oldFileInfo);
    }
}

void DoubleFolderPanel::onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "DoubleFolderPanel::onRightCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR && activeForm->objectName() == "r_folderForm")
    {
        emitCurrentChanged(newFileInfo, oldFileInfo);
    }
}

void DoubleFolderPanel::onFocusChanged(QWidget* oldWidget, QWidget* nowWidget)
{
    qDebug() << "DoubleFolderPanel::onFocusChanged(" <<
                ((oldWidget == nullptr) ? "null" : oldWidget->objectName()) << ", " <<
                ((nowWidget == nullptr) ? "null" : nowWidget->objectName()) << ")";

    if(nowWidget != nullptr)
    {
        FolderView* newFolderView = dynamic_cast<FolderView*>(nowWidget);
        if(newFolderView != nullptr && newFolderView->objectName() == "folderView")
        {
            FolderModel* folderModel = dynamic_cast<FolderModel*>(newFolderView->model());
            if(folderModel != Q_NULLPTR)
            {
                emitCurrentChanged(folderModel->fileInfo(newFolderView->currentIndex()), QFileInfo());
            }
        }
    }
}

void DoubleFolderPanel::emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    emit currentChanged(newFileInfo, oldFileInfo);
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

}           // namespace Farman
