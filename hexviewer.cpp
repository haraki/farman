#include "hexviewer.h"
#include "ui_hexviewer.h"

namespace Farman
{

HexViewer::HexViewer(const QString& filePath, QWidget *parent/* = Q_NULLPTR*/) :
    ViewerBase(filePath, parent),
    ui(new Ui::HexViewer)
{
    ui->setupUi(this);
}

HexViewer::~HexViewer()
{
    delete ui;
}

int HexViewer::setData()
{
    ui->hexView->setData(new QHexView::DataStorageArray(m_buffer));

    return 0;
}

}           // namespace Farman
