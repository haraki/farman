#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QMimeDatabase>
#include "fileattributesdialog.h"
#include "ui_fileattributesdialog.h"
#include "win32.h"

namespace Farman
{

FileAttributesDialog::FileAttributesDialog(const QFileInfo& fileInfo,
                                           QFile::Permissions permissions,
#ifdef Q_OS_WIN
                                           WinFileAttrFlags fileAttrFlags,
#endif
                                           const QDateTime& created,
                                           const QDateTime& lastModified,
                                           const qint64 fileSizeOnDisk,
                                           QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::FileAttributesDialog),
    m_permissions(permissions),
#ifdef Q_OS_WIN
    m_fileAttrFlags(fileAttrFlags),
#endif
    m_created(created),
    m_lastModified(lastModified)
{
    ui->setupUi(this);

    ui->fileNameLabel->setText(fileInfo.fileName());

    ui->informationFullPathLabel->setText(fileInfo.absoluteFilePath());
    ui->informationMimeLabel->setText(QMimeDatabase().mimeTypeForFile(fileInfo).name());
    if(fileInfo.isDir())
    {
        ui->informationFileSizeLabel->setText("- ");
    }
    else
    {
        ui->informationFileSizeLabel->setText(QString("%L1 ").arg(fileInfo.size()) + tr("Bytes"));
    }
    if(fileInfo.isDir() || fileSizeOnDisk < 0)
    {
        ui->informationSizeOnDiskLabel->setText("- ");
    }
    else
    {
        ui->informationSizeOnDiskLabel->setText(QString("%L1 ").arg(fileSizeOnDisk) + tr("Bytes"));
    }

    ui->ownershipUserLabel->setText(fileInfo.owner());
    ui->ownershipGroupLabel->setText(fileInfo.group());
#ifdef Q_OS_WIN
    ui->permissionsWinGroupBox->setVisible(true);
    ui->permissionsGroupBox->setVisible(false);

    ui->permissionsWinReadOnlyCheckBox->setChecked((fileAttrFlags & WinFileAttrFlag::ReadOnly) == static_cast<int>(WinFileAttrFlag::ReadOnly));
    ui->permissionsWinHiddenCheckBox->setChecked((fileAttrFlags & WinFileAttrFlag::Hidden) == static_cast<int>(WinFileAttrFlag::Hidden));
    ui->permissionsWinSystemCheckBox->setChecked((fileAttrFlags & WinFileAttrFlag::System) == static_cast<int>(WinFileAttrFlag::System));
    ui->permissionsWinArchiveCheckBox->setChecked((fileAttrFlags & WinFileAttrFlag::Archive) == static_cast<int>(WinFileAttrFlag::Archive));
#else
    ui->permissionsGroupBox->setVisible(true);
    ui->permissionsWinGroupBox->setVisible(false);

    ui->permissionsOwnerReadCheckBox->setChecked((permissions & QFile::Permission::ReadOwner) == QFile::Permission::ReadOwner);
    ui->permissionsOwnerWriteCheckBox->setChecked((permissions & QFile::Permission::WriteOwner) == QFile::Permission::WriteOwner);
    ui->permissionsOwnerExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeOwner) == QFile::Permission::ExeOwner);

    ui->permissionsGroupReadCheckBox->setChecked((permissions & QFile::Permission::ReadGroup) == QFile::Permission::ReadGroup);
    ui->permissionsGroupWriteCheckBox->setChecked((permissions & QFile::Permission::WriteGroup) == QFile::Permission::WriteGroup);
    ui->permissionsGroupExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeGroup) == QFile::Permission::ExeGroup);

    ui->permissionsOtherReadCheckBox->setChecked((permissions & QFile::Permission::ReadOther) == QFile::Permission::ReadOther);
    ui->permissionsOtherWriteCheckBox->setChecked((permissions & QFile::Permission::WriteOther) == QFile::Permission::WriteOther);
    ui->permissionsOtherExecuteCheckBox->setChecked((permissions & QFile::Permission::ExeOther) == QFile::Permission::ExeOther);
#endif
    ui->timestampCreatedDateTimeEdit->setDateTime(created);
    ui->timestampLastModifiedDateTimeEdit->setDateTime(lastModified);

    QRect geo = this->geometry();
    geo.setHeight(0);
    setMinimumSize(0, geo.height());
    setMaximumSize(geo.width(), geo.height());
}

FileAttributesDialog::~FileAttributesDialog()
{
    delete ui;
}

QFile::Permissions FileAttributesDialog::getPermissions() const
{
    return m_permissions;
}

#ifdef Q_OS_WIN
WinFileAttrFlags FileAttributesDialog::getFileAttrFlags() const
{
    return m_fileAttrFlags;
}
#endif

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
#ifdef Q_OS_WIN
    m_fileAttrFlags = WinFileAttrFlag::None;

    if(ui->permissionsWinReadOnlyCheckBox->isChecked())
    {
        m_fileAttrFlags |= WinFileAttrFlag::ReadOnly;
    }
    if(ui->permissionsWinHiddenCheckBox->isChecked())
    {
        m_fileAttrFlags |= WinFileAttrFlag::Hidden;
    }
    if(ui->permissionsWinSystemCheckBox->isChecked())
    {
        m_fileAttrFlags |= WinFileAttrFlag::System;
    }
    if(ui->permissionsWinArchiveCheckBox->isChecked())
    {
        m_fileAttrFlags |= WinFileAttrFlag::Archive;
    }
#else
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
#endif
    m_created = ui->timestampCreatedDateTimeEdit->dateTime();
    m_lastModified = ui->timestampLastModifiedDateTimeEdit->dateTime();

    QDialog::accept();
}

}           // namespace Farman
