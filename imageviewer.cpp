#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QBitmap>
#include <QBuffer>
#include <QMovie>
#include <QGraphicsProxyWidget>
#include "imageviewer.h"
#include "ui_imageviewer.h"
#include "settings.h"

namespace Farman
{

ImageViewer::ImageViewer(const QString& filePath, QWidget *parent) :
    ViewerBase(filePath, parent),
    ui(new Ui::ImageViewer),
    m_transparentBgRectItem(Q_NULLPTR),
    m_item(Q_NULLPTR)
{
    ui->setupUi(this);

    makeScaleComboBox("100");

    ui->fitInViewCheckBox->setChecked(Settings::getInstance()->getImageViewerFitInView());

    ui->imageGraphicsView->setScene(&m_scene);
    ui->imageGraphicsView->setFocus();
}

ImageViewer::~ImageViewer()
{
    qDebug() << "~ImageViewer()";

    delete ui;
}

void ImageViewer::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);

    if(ui->fitInViewCheckBox->isChecked())
    {
        autoScale();
    }
}

void ImageViewer::on_fitInViewCheckBox_stateChanged(int arg1)
{
    qDebug() << "on_fitInViewCheckBox_stateChanged";

    Q_UNUSED(arg1);

    Settings::getInstance()->setImageViewerFitInView(ui->fitInViewCheckBox->isChecked());

    ui->scaleComboBox->setEnabled(!ui->fitInViewCheckBox->isChecked());

    // Fit in view On 時、時々スクロールバーが表示されてしまうので、Off にする
    Qt::ScrollBarPolicy scrollBarPolicy = (ui->fitInViewCheckBox->isChecked()) ? Qt::ScrollBarAlwaysOff : Qt::ScrollBarAsNeeded;
    ui->imageGraphicsView->setVerticalScrollBarPolicy(scrollBarPolicy);
    ui->imageGraphicsView->setHorizontalScrollBarPolicy(scrollBarPolicy);

    if(ui->fitInViewCheckBox->isChecked())
    {
        autoScale();
    }
}

void ImageViewer::on_scaleComboBox_editTextChanged(const QString &arg1)
{
    qDebug() << "on_scaleComboBox_editTextChanged";

    if(!ui->fitInViewCheckBox->isChecked())
    {
        bool ok = false;
        float scale = arg1.toFloat(&ok);
        if(ok && scale > 0.f)
        {
            scale /= 100.f;

            setScale(scale);
        }
    }
}

void ImageViewer::on_scaleComboBox_activated(const QString &arg1)
{
    qDebug() << "on_scaleComboBox_activated";

    if(!ui->fitInViewCheckBox->isChecked())
    {
        bool ok = false;
        float scale = arg1.toFloat(&ok);
        if(!ok || scale <= 0.f)
        {
            ui->scaleComboBox->removeItem(ui->scaleComboBox->currentIndex());

            return;
        }

        makeScaleComboBox(arg1);
    }
}

void ImageViewer::makeScaleComboBox(const QString& scaleStr)
{
    static const QList<QString> presetScaleList =
    {
        "25",
        "50",
        "75",
        "100",
        "150",
        "200",
    };
    ui->scaleComboBox->clear();
    ui->scaleComboBox->addItems(presetScaleList);

    float scale = scaleStr.toFloat();
    int index;
    for(index = 0;index < presetScaleList.size();index++)
    {
        float presetScale = presetScaleList[index].toFloat();

        if(scale == presetScale)
        {
            break;
        }
        else if(scale < presetScale)
        {
            ui->scaleComboBox->insertItem(index, scaleStr);

            break;
        }
    }

    if(index == presetScaleList.size())
    {
        ui->scaleComboBox->insertItem(index, scaleStr);
    }

    ui->scaleComboBox->setCurrentIndex(index);
}

int ImageViewer::setData()
{
    ImageViewer::ItemWidget* itemWidget = new ImageViewer::ItemWidget(m_buffer);
    if(itemWidget == Q_NULLPTR)
    {
        return -1;
    }

    m_item = m_scene.addWidget(itemWidget);

    if(m_item == Q_NULLPTR)
    {
        return -1;
    }

    QSizeF itemSize = m_item->boundingRect().size();

    m_transparentBgRectItem = m_scene.addRect(0, 0, itemSize.width(), itemSize.height(), QPen(QBrush(), 0, Qt::NoPen), createTransparentBGBrush());
    if(m_transparentBgRectItem == Q_NULLPTR)
    {
        return -1;
    }
    m_transparentBgRectItem->setZValue(m_item->zValue() - 1);

    ui->sizeLabel->setText(QString("%1 x %2").arg(itemSize.width()).arg(itemSize.height()));

    if(ui->fitInViewCheckBox->isChecked())
    {
        autoScale();
    }
    else
    {
        bool ok = false;
        float scale = ui->scaleComboBox->currentText().toFloat(&ok);
        if(ok && scale > 0.f)
        {
            scale /= 100.f;

            setScale(scale);
        }
    }

    return 0;
}

void ImageViewer::autoScale()
{
    if(m_item == Q_NULLPTR)
    {
        return;
    }

    // 拡縮率を自動算出
    QSizeF itemSize = m_item->boundingRect().size();
    QSize viewSize = ui->imageGraphicsView->size();

    float wScale = viewSize.width() / itemSize.width();
    float hScale = viewSize.height() / itemSize.height();
    float scale = (wScale <= hScale) ? wScale : hScale;

    setScale(scale);

    makeScaleComboBox(QString::number(scale * 100.f, 'f', 1));
}

void ImageViewer::setScale(float scale)
{
    if(m_item == Q_NULLPTR)
    {
        return;
    }

    m_item->setScale(scale);

    QRectF sceneRect = m_item->boundingRect();
    sceneRect.setWidth(sceneRect.width() * scale);
    sceneRect.setHeight(sceneRect.height() * scale);
    m_scene.setSceneRect(sceneRect);

    m_transparentBgRectItem->setRect(sceneRect);
}

QBrush ImageViewer::createTransparentBGBrush()
{
    QBrush bgBrush = QBrush();

    if(Settings::getInstance()->getImageViewerBGType() == ImageViewerBGType::Solid)
    {
        bgBrush.setStyle(Qt::SolidPattern);
    }
    else
    {
        bgBrush.setStyle(Qt::TexturePattern);

        const uchar bgBitmapBits[] = {
            0xFF,0x00,
            0xFF,0x00,
            0xFF,0x00,
            0xFF,0x00,
            0xFF,0x00,
            0xFF,0x00,
            0xFF,0x00,
            0xFF,0x00,
            0x00,0xFF,
            0x00,0xFF,
            0x00,0xFF,
            0x00,0xFF,
            0x00,0xFF,
            0x00,0xFF,
            0x00,0xFF,
            0x00,0xFF,
        };
        QBitmap bgBitmap = QBitmap::fromData({16, 16}, bgBitmapBits);

        bgBrush.setTexture(bgBitmap);
    }

    bgBrush.setColor(Settings::getInstance()->getColorSetting("imageViewer_background"));

    return bgBrush;
}

ImageViewer::ItemWidget::ItemWidget(QByteArray& buffer) :
    m_buffer(Q_NULLPTR),
    m_movie(Q_NULLPTR)
{
    qDebug() << "ImageViewer::ItemWidget::ItemWidget()";

    m_buffer = new QBuffer(&buffer);
    Q_ASSERT(m_buffer);

    m_movie = new QMovie(m_buffer);
    Q_ASSERT(m_movie);

    setMovie(m_movie);

    m_movie->start();

    setFixedSize(m_movie->frameRect().size());          // QMovie::start() の後でないとサイズは取得できない
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

ImageViewer::ItemWidget::~ItemWidget()
{
    qDebug() << "ImageViewer::ItemWidget::~ItemWidget()";

    if(m_movie != Q_NULLPTR)
    {
        delete m_movie;
        m_movie = Q_NULLPTR;
    }

    if(m_buffer != Q_NULLPTR)
    {
        delete m_buffer;
        m_buffer = Q_NULLPTR;
    }
}

}
