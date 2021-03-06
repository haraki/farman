﻿#ifndef FILEATTRIBUTESDIALOG_H
#define FILEATTRIBUTESDIALOG_H

#include <QDialog>
#include <QFile>
#include <QDateTime>
#include "types.h"

namespace Ui {
class FileAttributesDialog;
}

class QFileInfo;

namespace Farman
{

class FileAttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileAttributesDialog(const QFileInfo& fileInfo,
                                  QFile::Permissions permissions,
#ifdef Q_OS_WIN
                                  WinFileAttrFlags fileAttrFlags,
#endif
                                  const QDateTime& created,
                                  const QDateTime& lastModified,
                                  const qint64 fileSizeOnDisk,
                                  QWidget *parent = Q_NULLPTR);
    ~FileAttributesDialog();

    QFile::Permissions getPermissions() const;
#ifdef Q_OS_WIN
    WinFileAttrFlags getFileAttrFlags() const;
#endif
    QDateTime getCreated() const;
    QDateTime getLastModified() const;

    void accept();

private:
    Ui::FileAttributesDialog *ui;

    QFile::Permissions m_permissions;
#ifdef Q_OS_WIN
    WinFileAttrFlags m_fileAttrFlags;
#endif
    QDateTime m_created;
    QDateTime m_lastModified;
};

}           // namespace Farman

#endif // FILEATTRIBUTESDIALOG_H
