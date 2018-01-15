#include "fileattributesdialog.h"
#include "ui_fileattributesdialog.h"

namespace Farman
{

FileAttributesDialog::FileAttributesDialog(const QString& fileName,
                                           const QString& userName,
                                           const QString& groupName,
                                           QFile::Permissions permissions,
                                           const QDateTime& created,
                                           const QDateTime& lastModified,
                                           QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::FileAttributesDialog)
{
    ui->setupUi(this);

    ui->fileNameLabel->setText(fileName);

    ui->ownershipUserLabel->setText(userName);
    ui->ownershipGroupLabel->setText(groupName);

    // チェックボックスをマウスクリックできないようにするための措置
    ui->permissionsOwnerReadCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->permissionsOwnerWriteCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->permissionsOwnerExecuteCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->permissionsGroupReadCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->permissionsGroupWriteCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->permissionsGroupExecuteCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->permissionsOtherReadCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->permissionsOtherWriteCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->permissionsOtherExecuteCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->permissionsOwnerReadCheckBox->setChecked((permissions & QFile::Permission::ReadOwner) == QFile::Permission::ReadOwner);
    ui->permissionsOwnerWriteCheckBox->setChecked((permissions & QFile::Permission::WriteOwner) == QFile::Permission::WriteOwner);
    ui->permissionsOwnerExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeOwner) == QFile::Permission::ExeOwner);

    ui->permissionsGroupReadCheckBox->setChecked((permissions & QFile::Permission::ReadGroup) == QFile::Permission::ReadGroup);
    ui->permissionsGroupWriteCheckBox->setChecked((permissions & QFile::Permission::WriteGroup) == QFile::Permission::WriteGroup);
    ui->permissionsGroupExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeGroup) == QFile::Permission::ExeGroup);

    ui->permissionsOtherReadCheckBox->setChecked((permissions & QFile::Permission::ReadOther) == QFile::Permission::ReadOther);
    ui->permissionsOtherWriteCheckBox->setChecked((permissions & QFile::Permission::WriteOther) == QFile::Permission::WriteOther);
    ui->permissionsOtherExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeOther) == QFile::Permission::ExeOther);

    ui->timestampCreatedLabel->setText(created.toString(Qt::DefaultLocaleLongDate));
    ui->timestampLastModifiedLabel->setText(lastModified.toString(Qt::DefaultLocaleLongDate));
}

FileAttributesDialog::~FileAttributesDialog()
{
    delete ui;
}

}           // namespace Farman
