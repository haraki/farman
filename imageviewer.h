#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QGraphicsScene>
#include "viewerbase.h"

class QString;
class QBrush;
class QPixmap;
class QByteArray;

namespace Ui {
class ImageViewer;
}

namespace Farman
{

class ImageViewer : public ViewerBase
{
    Q_OBJECT

public:
    explicit ImageViewer(const QString& filePath, QWidget *parent = Q_NULLPTR);
    ~ImageViewer();

private Q_SLOTS:
    void on_fitInViewCheckBox_stateChanged(int arg1);
    void on_scaleComboBox_editTextChanged(const QString &arg1);
    void on_scaleComboBox_activated(const QString &arg1);

private:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void makeScaleComboBox(const QString& scaleStr);
    int setData() Q_DECL_OVERRIDE;
    void autoScale();
    void setScale(float scale);
    QBrush createTransparentBGBrush();

    Ui::ImageViewer *ui;

    QGraphicsScene m_scene;
    QGraphicsPixmapItem* m_pixmapItem;
    QGraphicsRectItem* m_transparentBgRectItem;             // PNG等透過部分がある画像の背景
};

}

#endif // IMAGEVIEWER_H
