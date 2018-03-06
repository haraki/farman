#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QDebug>

class QString;
class QImageReader;
class QPixmap;

namespace Ui {
class ImageViewer;
}

namespace Farman
{

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(const QString& filePath, QWidget *parent/* = Q_NULLPTR*/);
    ~ImageViewer();

Q_SIGNALS:
    void closeViewer(const QString& viewerName);

private Q_SLOTS:
    void on_autoScaleCheckBox_stateChanged(int arg1);
    void on_scaleComboBox_editTextChanged(const QString &arg1);
    void on_scaleComboBox_activated(const QString &arg1);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void makeScaleComboBox(const QString& scaleStr);
    void setData();
    void setScale(float scale);
    void autoScale();

    void emitCloseViewer(const QString& viewerName);

    Ui::ImageViewer *ui;

    QString m_filePath;
    QImageReader* m_imageReader;

    QGraphicsScene m_scene;
};

}

#endif // IMAGEVIEWER_H
