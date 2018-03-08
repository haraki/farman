#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QGraphicsScene>

class QString;
class QPixmap;
class QByteArray;
class QProgressDialog;

namespace Ui {
class ImageViewer;
}

namespace Farman
{

class ReadFileWorker;

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(const QString& filePath, QWidget *parent/* = Q_NULLPTR*/);
    ~ImageViewer();

    int start();

Q_SIGNALS:
    void closeViewer(const QString& viewerName);

private Q_SLOTS:
    void onReadFileFinished(int result);
    void onReadFileError(const QString& err);

    void onProgressDialogCanceled();

    void on_fitInViewCheckBox_stateChanged(int arg1);
    void on_scaleComboBox_editTextChanged(const QString &arg1);
    void on_scaleComboBox_activated(const QString &arg1);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void makeScaleComboBox(const QString& scaleStr);
    int setData();
    void setScale(float scale);
    void autoScale();

    void emitCloseViewer(const QString& viewerName);

    Ui::ImageViewer *ui;

    QString m_filePath;
    QByteArray m_buffer;
    ReadFileWorker* m_worker;
    QProgressDialog* m_progressDialog;

    QGraphicsScene m_scene;
};

}

#endif // IMAGEVIEWER_H
