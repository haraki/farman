#include "overwritedialog.h"
#include "ui_overwritedialog.h"

namespace Farman
{

OverwriteDialog::OverwriteDialog(OverwriteMethodType methodType, const QString& renameText, QWidget *parent/* = Q_NULLPTR*/)
    : QDialog(parent)
    , ui(new Ui::OverwriteDialog)
    , m_methodType(methodType)
    , m_renameFileName(renameText)
{
    ui->setupUi(this);
}

OverwriteDialog::~OverwriteDialog()
{
    delete ui;
}

}           // namespace Farman
