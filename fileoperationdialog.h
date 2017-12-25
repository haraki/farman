#ifndef FILEOPERATIONDIALOG_H
#define FILEOPERATIONDIALOG_H

#include <QDialog>
#include <QFileInfo>

namespace Ui {
class FileOperationDialog;
}

namespace Farman
{

class FileOperationDialog : public QDialog
{
    Q_OBJECT

public:
    enum class OperationType : int
    {
        Copy,
        Move,

        OperationTypeNum,
    };

public:
    explicit FileOperationDialog(OperationType type,
                                 const QString& srcDirPath,
                                 const QList<QFileInfo>& srcFileInfos,
                                 const QString& dstDirPath,
                                 QWidget *parent = Q_NULLPTR);
    ~FileOperationDialog();

    QString getDstDirPath() const;

private slots:
    void on_dstFolderPathSelectButton_clicked();

private:
    explicit FileOperationDialog(QWidget *parent = 0);

    Ui::FileOperationDialog *ui;
};

}       // namespace Farman

#endif // FILEOPERATIONDIALOG_H
