﻿#ifndef OVERWRITEDIALOG_H
#define OVERWRITEDIALOG_H

#include <QDialog>
#include "types.h"

namespace Ui {
class OverwriteDialog;
}

class QFileInfo;

namespace Farman
{

class OverwriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OverwriteDialog(const QString& srcFilePath,
                             const QString& dstFilePath,
                             OverwriteMethodType methodType,
                             QWidget *parent = Q_NULLPTR);
    ~OverwriteDialog() Q_DECL_OVERRIDE;

    OverwriteMethodType getMethodType() const;
    QString getRenameFileName() const;
    bool getKeepSetting() const;

private slots:
    void on_methodOverwriteRadioButton_clicked();
    void on_methodOverwriteIfNewerRadioButton_clicked();
    void on_methodSkipRadioButton_clicked();
    void on_methodRenameRadioButton_clicked();

private:
    void accept() Q_DECL_OVERRIDE;

    Ui::OverwriteDialog *ui;

    OverwriteMethodType m_methodType;
    QString m_renameFileName;
    bool m_keepSetting;
};

}           // namespace Farman

#endif // OVERWRITEDIALOG_H
