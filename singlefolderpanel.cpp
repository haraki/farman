#include <QGridLayout>
#include "singlefolderpanel.h"
#include "ui_singlefolderpanel.h"
#include "folderform.h"
#include "folderview.h"

SingleFolderPanel::SingleFolderPanel(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent/* = Q_NULLPTR*/)
    : FolderPanelBase(parent)
    , ui(new Ui::SingleFolderPanel)
{
    ui->setupUi(this);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setSpacing(6);
    vLayout->setObjectName(QStringLiteral("vLayout"));
    vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* folderForm = new FolderForm(filterFlags, sortFlags, this);
    folderForm->setObjectName(QStringLiteral("folderForm"));
    folderForm->setPath(path);
    vLayout->addWidget(folderForm);

    this->setLayout(vLayout);

    FolderView* folderView = folderForm->findChild<FolderView*>("folderView");
    if(folderView != Q_NULLPTR)
    {
        folderView->setFocus();
    }
}

SingleFolderPanel::~SingleFolderPanel()
{
    delete ui;
}

FolderForm* SingleFolderPanel::getFolderForm()
{
    return findChild<FolderForm*>(QStringLiteral("folderForm"));
}
