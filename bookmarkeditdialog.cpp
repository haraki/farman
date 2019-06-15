#include <QFileDialog>
#include <QPushButton>
#include <QStyle>
#include <QDebug>
#include "bookmarkeditdialog.h"
#include "ui_bookmarkeditdialog.h"

namespace Farman
{

BookmarkEditDialog::BookmarkEditDialog(const QString& currentDirPath, const BookmarkInfo& info/* = BookmarkInfo()*/, QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::BookmarkEditDialog),
    m_currentDirPath(currentDirPath),
    m_info(info)
{
    ui->setupUi(this);

    ui->selectPathToolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogStart));

    ui->nameLineEdit->setText(info.getName());
    ui->pathLineEdit->setText(info.getPath());

    checkNamePathEmpty();
}

BookmarkEditDialog::~BookmarkEditDialog()
{
    delete ui;
}

void BookmarkEditDialog::accept()
{
    m_info.setName(ui->nameLineEdit->text());
    m_info.setPath(ui->pathLineEdit->text());

    QDialog::accept();
}

void BookmarkEditDialog::on_nameLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    checkNamePathEmpty();
}

void BookmarkEditDialog::on_selectPathToolButton_clicked()
{
    QString dirPath = ui->pathLineEdit->text();

    if(dirPath.isEmpty())
    {
        dirPath = m_currentDirPath;
    }

    dirPath = QFileDialog::getExistingDirectory(this,
                                                tr("Select folder."),
                                                dirPath,
                                                QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(!dirPath.isEmpty())
    {
        ui->pathLineEdit->setText(dirPath);
    }

    checkNamePathEmpty();
}

void BookmarkEditDialog::checkNamePathEmpty()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!ui->nameLineEdit->text().isEmpty() && !ui->pathLineEdit->text().isEmpty());
}

}           // namespace Farman
