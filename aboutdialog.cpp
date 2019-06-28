#include <QMessageBox>
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "licensedialog.h"

#ifdef Q_OS_WIN
#   include "win32.h"
#else
#   include "xnix.h"
#endif

namespace Farman
{

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->appNameLabel->setText(tr("<p><font size = '+4'>%1<font size = '+1'> Version %2</font></font></p>")
                              .arg(qApp->applicationName())
                              .arg(qApp->applicationVersion()));

    ui->appDescriptionLabel->setText(tr("<p>Based on Qt %1 ").arg(qVersion())
                                     + tr("(%1)").arg(getCompilerVersion())
                                     + tr("<p>Copyright 2019 %1 All right reserved.</p>").arg(qApp->organizationName()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_aboutQtPushButton_clicked()
{
    QMessageBox::aboutQt(this);
}

void AboutDialog::on_licensePushButton_clicked()
{
    LicenseDialog(this).exec();
}

}           // namespace Farman
