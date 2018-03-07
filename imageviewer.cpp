#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>
#include <QByteArray>
#include <QDebug>
#include <QProgressDialog>
#include "imageviewer.h"
#include "ui_imageviewer.h"
#include "mainwindow.h"
#include "readfileworker.h"

namespace Farman
{

ImageViewer::ImageViewer(const QString& filePath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageViewer),
    m_filePath(filePath),
    m_buffer(),
    m_worker(Q_NULLPTR),
    m_progressDialog(Q_NULLPTR)
{
    ui->setupUi(this);

    connect(this,
            SIGNAL(closeViewer(const QString)),
            MainWindow::getInstance(),
            SLOT(onCloseViewer(const QString)));

    makeScaleComboBox("100");

    ui->scaleComboBox->setEnabled(!ui->autoScaleCheckBox->isChecked());

    ui->imageGraphicsView->setScene(&m_scene);
    ui->imageGraphicsView->setFocus();
}

ImageViewer::~ImageViewer()
{
    qDebug() << "~ImageViewer()";

    delete ui;

    delete m_progressDialog;
}

int ImageViewer::start()
{
    m_buffer.clear();

    m_progressDialog = new QProgressDialog(tr("%1 reading").arg(m_filePath), tr("Cancel"), 0, 1, this);
    if(m_progressDialog == Q_NULLPTR)
    {
        return -1;
    }

    connect(m_progressDialog,
            SIGNAL(canceled()),
            this,
            SLOT(onProgressDialogCanceled()));

    m_worker = new ReadFileWorker(m_filePath, &m_buffer);
    if(m_worker == Q_NULLPTR)
    {
        return -1;
    }

    connect(m_worker,
            SIGNAL(start(int,int)),
            m_progressDialog,
            SLOT(setRange(int,int)));
    connect(m_worker,
            SIGNAL(progress(int)),
            m_progressDialog,
            SLOT(setValue(int)));
    connect(m_worker,
            SIGNAL(finished(int)),
            this,
            SLOT(onReadFileFinished(int)));
    connect(m_worker,
            SIGNAL(error(QString)),
            this,
            SLOT(onReadFileError(QString)));

    m_progressDialog->show();
    m_worker->exec();

    return 0;
}

bool ImageViewer::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type())
    {
    case QEvent::KeyPress:
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(e);
        if(keyEvent != Q_NULLPTR)
        {
            Qt::Key key = static_cast<Qt::Key>(keyEvent->key());

            qDebug() << "ImageViewer::eventFilter : " << key;

            switch(key)
            {
            case Qt::Key_Return:
                // Return は Designer のショートカットの設定では効かないようなので、ハードコーディングする

                // TODO:
                // このタイミングで Viewer のインスタンスが delete される
                // そのため、以降関数を抜けるまでの間に Viewer のメンバにアクセスしてはならない
                // 何とかしたいがいい方法が見つからないので、ひとまずこのままとする
                // MainWindow 側で Key_Return 押下イベントを検知できればよいのだが…
                emitCloseViewer(objectName());

                ret = true;

                break;

            default:
                break;
            }
        }

        break;
    }
    default:
        break;
    }

    return ret;
}

void ImageViewer::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);

    if(ui->autoScaleCheckBox->isChecked())
    {
        autoScale();
    }
}

void ImageViewer::onReadFileFinished(int result)
{
    qDebug() << "ViewerPanel::onReadFileFinished : result : " << result;

    m_worker = Q_NULLPTR;           // deleteLater で自動的にデストラクトされているので、Q_NULLPTR を代入するのみ

    if(result == 0)
    {
        setData();
    }
    else
    {
        closeViewer(objectName());
    }
}

void ImageViewer::onReadFileError(const QString& err)
{
    qDebug() << "ViewerPanel::onReadFileError : err : " << err;
}

void ImageViewer::onProgressDialogCanceled()
{
    qDebug() << "ImageViewer::onProgressDialogCanceled()";

    m_worker->abort();
}

void ImageViewer::on_autoScaleCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    ui->scaleComboBox->setEnabled(!ui->autoScaleCheckBox->isChecked());

    // AutoScale On 時、時々スクロールバーが表示されてしまうので、Off にする
    Qt::ScrollBarPolicy scrollBarPolicy = (ui->autoScaleCheckBox->isChecked()) ? Qt::ScrollBarAlwaysOff : Qt::ScrollBarAsNeeded;
    ui->imageGraphicsView->setVerticalScrollBarPolicy(scrollBarPolicy);
    ui->imageGraphicsView->setHorizontalScrollBarPolicy(scrollBarPolicy);

    if(ui->autoScaleCheckBox->isChecked())
    {
        autoScale();
    }
}

void ImageViewer::on_scaleComboBox_editTextChanged(const QString &arg1)
{
    qDebug() << "on_scaleComboBox_editTextChanged";

    if(!ui->autoScaleCheckBox->isChecked())
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

    if(!ui->autoScaleCheckBox->isChecked())
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
    QPixmap pixmap = QPixmap();
    if(!pixmap.loadFromData(m_buffer))
    {
        return -1;
    }

    m_scene.addPixmap(pixmap);

    ui->sizeLabel->setText(QString("%1 x %2").arg(m_scene.sceneRect().width()).arg(m_scene.sceneRect().height()));

    if(ui->autoScaleCheckBox->isChecked())
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
    QSizeF sceneSize = m_scene.sceneRect().size();
    QSize viewSize = ui->imageGraphicsView->size();

    // QGraphicsView::fitInView() だと画像の上下左右に隙間ができるので、自前で算出する
    float wScale = viewSize.width() / sceneSize.width();
    float hScale = viewSize.height() / sceneSize.height();
    float scale = (wScale <= hScale) ? wScale : hScale;

    setScale(scale);

    qDebug() << m_scene.sceneRect() << ", " << ui->imageGraphicsView->size() << ", " << scale;

    makeScaleComboBox(QString::number(scale * 100.f, 'f', 1));
}

void ImageViewer::setScale(float scale)
{
    ui->imageGraphicsView->resetMatrix();
    ui->imageGraphicsView->scale(scale, scale);
}

void ImageViewer::emitCloseViewer(const QString& viewerName)
{
    emit closeViewer(viewerName);
}

}
