#include <QGridLayout>
#include "singlepanelform.h"
#include "ui_singlepanelform.h"
#include "folderform.h"

SinglePanelForm::SinglePanelForm(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::SinglePanelForm)
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
}

SinglePanelForm::~SinglePanelForm()
{
    delete ui;
}
