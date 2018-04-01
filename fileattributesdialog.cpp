﻿#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMimeDatabase>
#include "fileattributesdialog.h"
#include "ui_fileattributesdialog.h"

namespace Farman
{

FileAttributesDialog::FileAttributesDialog(const QFileInfo& fileInfo,
                                           QFile::Permissions permissions,
                                           const QDateTime& created,
                                           const QDateTime& lastModified,
                                           QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::FileAttributesDialog),
    m_permissions(permissions),
    m_created(created),
    m_lastModified(lastModified)
{
    ui->setupUi(this);

    ui->fileNameLabel->setText(fileInfo.fileName());

    ui->informationFullPathLabel->setText(fileInfo.absoluteFilePath());
    ui->informationMimeLabel->setText(QMimeDatabase().mimeTypeForFile(fileInfo).name());

    ui->ownershipUserLabel->setText(fileInfo.owner());
    ui->ownershipGroupLabel->setText(fileInfo.group());

    ui->permissionsOwnerReadCheckBox->setChecked((permissions & QFile::Permission::ReadOwner) == QFile::Permission::ReadOwner);
    ui->permissionsOwnerWriteCheckBox->setChecked((permissions & QFile::Permission::WriteOwner) == QFile::Permission::WriteOwner);
    ui->permissionsOwnerExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeOwner) == QFile::Permission::ExeOwner);

    ui->permissionsGroupReadCheckBox->setChecked((permissions & QFile::Permission::ReadGroup) == QFile::Permission::ReadGroup);
    ui->permissionsGroupWriteCheckBox->setChecked((permissions & QFile::Permission::WriteGroup) == QFile::Permission::WriteGroup);
    ui->permissionsGroupExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeGroup) == QFile::Permission::ExeGroup);

    ui->permissionsOtherReadCheckBox->setChecked((permissions & QFile::Permission::ReadOther) == QFile::Permission::ReadOther);
    ui->permissionsOtherWriteCheckBox->setChecked((permissions & QFile::Permission::WriteOther) == QFile::Permission::WriteOther);
    ui->permissionsOtherExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeOther) == QFile::Permission::ExeOther);

    ui->timestampCreatedDateTimeEdit->setDateTime(created);
    ui->timestampLastModifiedDateTimeEdit->setDateTime(lastModified);
}

FileAttributesDialog::~FileAttributesDialog()
{
    delete ui;
}

QFile::Permissions FileAttributesDialog::getPermissions() const
{
    return m_permissions;
}

QDateTime FileAttributesDialog::getCreated() const
{
    return m_created;
}

QDateTime FileAttributesDialog::getLastModified() const
{
    return m_lastModified;
}

void FileAttributesDialog::accept()
{
    m_permissions = 0;

    if(ui->permissionsOwnerReadCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::ReadOwner;
    }
    if(ui->permissionsOwnerWriteCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::WriteOwner;
    }
    if(ui->permissionsOwnerExecuteCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::ExeOwner;
    }

    if(ui->permissionsGroupReadCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::ReadGroup;
    }
    if(ui->permissionsGroupWriteCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::WriteGroup;
    }
    if(ui->permissionsGroupExecuteCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::ExeGroup;
    }

    if(ui->permissionsOtherReadCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::ReadOther;
    }
    if(ui->permissionsOtherWriteCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::WriteOther;
    }
    if(ui->permissionsOtherExecuteCheckBox->isChecked())
    {
        m_permissions |= QFile::Permission::ExeOther;
    }

    qDebug() << "permissions == " << m_permissions;

    m_created = ui->timestampCreatedDateTimeEdit->dateTime();
    m_lastModified = ui->timestampLastModifiedDateTimeEdit->dateTime();

    QDialog::accept();
}

}           // namespace Farman