#include <QFile>
#include <QDebug>
#include "licensedialog.h"
#include "ui_licensedialog.h"

LicenseDialog::LicenseDialog(QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::LicenseDialog)
{
    ui->setupUi(this);

    QFile licenseTxt(":/text/license.txt");
    if(licenseTxt.open(QFile::ReadOnly))
    {
        QString licenseHtml;

        while(!licenseTxt.atEnd())
        {
            licenseHtml.push_back(licenseTxt.readLine() + "<br />");
        }

        ui->licenseTextBrowser->setHtml(licenseHtml);

        licenseTxt.close();
    }
}

LicenseDialog::~LicenseDialog()
{
    delete ui;
}
