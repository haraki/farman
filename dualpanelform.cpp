#include <QKeyEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <qdir.h>
#include "dualpanelform.h"
#include "ui_dualpanelform.h"
#include "folderform.h"
#include "folderview.h"

DualPanelForm::DualPanelForm(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DualPanelForm)
{
    ui->setupUi(this);

    QVBoxLayout* l_vLayout = new QVBoxLayout();
    l_vLayout->setSpacing(6);
    l_vLayout->setObjectName(QStringLiteral("l_vLayout"));
    l_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* l_folderForm = new FolderForm(filterFlags, sortFlags, this);
    l_folderForm->setObjectName(QStringLiteral("l_folderForm"));
    l_folderForm->setPath(path);

    l_vLayout->addWidget(l_folderForm);

    ui->leftPanel->setLayout(l_vLayout);

    FolderView* l_folderView = l_folderForm->findChild<FolderView*>("folderView");
    if(l_folderView != nullptr)
    {
        l_folderView->installEventFilter(this);
    }

    QVBoxLayout* r_vLayout = new QVBoxLayout();
    r_vLayout->setSpacing(6);
    r_vLayout->setObjectName(QStringLiteral("r_vLayout"));
    r_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* r_folderForm = new FolderForm(filterFlags, sortFlags, this);
    r_folderForm->setObjectName(QStringLiteral("r_folderForm"));
    r_folderForm->setPath(path);

    r_vLayout->addWidget(r_folderForm);

    ui->rightPanel->setLayout(r_vLayout);

    FolderView* r_folderView = r_folderForm->findChild<FolderView*>("folderView");
    if(r_folderView != nullptr)
    {
        r_folderView->installEventFilter(this);
    }
}

DualPanelForm::~DualPanelForm()
{
    delete ui;
}

bool DualPanelForm::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type()) {
    case QEvent::KeyPress:
    {
        Qt::Key key = static_cast<Qt::Key>(dynamic_cast<QKeyEvent*>(e)->key());

        qDebug() << key;

        FolderForm* activeForm = getActiveFolderForm();
        if(activeForm == nullptr)
        {
            break;
        }

        switch(key)
        {
        case Qt::Key_Left:
            if(activeForm->objectName() == "l_folderForm")
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
            if(activeForm->objectName() == "r_folderForm")
            {
                activeForm->onGoToParent();
            }
            else
            {
                setActiveFolderForm("r_folderForm");
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

FolderForm* DualPanelForm::getActiveFolderForm()
{
    QWidget* fw = focusWidget();
    if(fw == nullptr)
    {
        return nullptr;
    }

    if(fw->objectName() != "folderView")
    {
        return nullptr;
    }

    return dynamic_cast<FolderForm*>(fw->parent());
}

void DualPanelForm::setActiveFolderForm(const QString& objectName)
{
    FolderForm* folderForm = findChild<FolderForm*>(objectName);
    if(folderForm != nullptr)
    {
        FolderView* folderView = folderForm->findChild<FolderView*>("folderView");
        if(folderView != nullptr)
        {
            folderView->setFocus();
        }
    }
}
