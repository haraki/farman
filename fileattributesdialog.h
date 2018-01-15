#ifndef FILEATTRIBUTESDIALOG_H
#define FILEATTRIBUTESDIALOG_H

#include <QDialog>
#include <QFile>
#include <QDateTime>

namespace Ui {
class FileAttributesDialog;
}

namespace Farman
{

class FileAttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileAttributesDialog(const QString& fileName,
                                  const QString& userName,
                                  const QString& groupName,
                                  QFile::Permissions permissions,
                                  const QDateTime& created,
                                  const QDateTime& lastModified,
                                  QWidget *parent = Q_NULLPTR);
    ~FileAttributesDialog();

private:
    Ui::FileAttributesDialog *ui;
};

}           // namespace Farman

#endif // FILEATTRIBUTESDIALOG_H
