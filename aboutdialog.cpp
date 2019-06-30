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

    ui->appNameLabel->setText(tr("<p><span style = 'font-size:xx-large; font-weight:bold'>%1 %2</span>")
                              .arg(qApp->applicationName())
                              .arg(qApp->applicationVersion())
#ifdef QT_DEBUG
                              + tr(" (Debug Build)")
#endif
                              + "</p>");

    ui->appDescriptionLabel->setText(tr("<p>Based on Qt %1 (%2)</p>").arg(qVersion()).arg(getCompilerVersion())
                                     + tr("<p>Revision : %1</p>").arg(APP_REVISION)
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
