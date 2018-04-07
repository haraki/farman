#include "hexviewer.h"
#include "ui_hexviewer.h"
#include "settings.h"

namespace Farman
{

HexViewer::HexViewer(const QString& filePath, QWidget *parent/* = Q_NULLPTR*/) :
    ViewerBase(filePath, parent),
    ui(new Ui::HexViewer)
{
    ui->setupUi(this);

    initFont();

    initPalette();

    ui->hexView->setFocus();
}

HexViewer::~HexViewer()
{
    delete ui;
}

void HexViewer::initFont()
{
    ui->hexView->setFont(Settings::getInstance()->getFontSetting("hexViewer"));
}

void HexViewer::initPalette()
{
    QPalette pal;

    pal = ui->hexView->palette();
    pal.setColor(QPalette::Text, Settings::getInstance()->getColorSetting("hexViewer_text"));
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("hexViewer_background"));
    ui->hexView->setPalette(pal);

    pal = ui->hexView->getAddressAreaPalette();
    pal.setColor(QPalette::Text, Settings::getInstance()->getColorSetting("hexViewer_address_text"));
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("hexViewer_address_background"));
    ui->hexView->setAddressAreaPalette(pal);
}

int HexViewer::setData()
{
    ui->hexView->setData(new QHexView::DataStorageArray(m_buffer));

    return 0;
}

}           // namespace Farman
