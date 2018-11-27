#ifndef VIEWERBASE_H
#define VIEWERBASE_H

#include <QWidget>

class QString;
class QByteArray;
class QProgressDialog;

namespace Farman
{

class MainWindow;
class ReadFileWorker;

class ViewerBase : public QWidget
{
    Q_OBJECT
public:
    explicit ViewerBase(const QString& filePath, QWidget *parent = Q_NULLPTR);
    virtual ~ViewerBase() Q_DECL_OVERRIDE;

    virtual int start(MainWindow* mainWindow);

Q_SIGNALS:
    void closeViewer(const QString& viewerName);

protected:
    void emitCloseViewer();

    QString m_filePath;
    QByteArray m_buffer;

private Q_SLOTS:
    void onReadFileFinished(int result);
    void onReadFileError(const QString& err);

    void onProgressDialogCanceled();

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;
    virtual int setData() = 0;

    ReadFileWorker* m_worker;
    QProgressDialog* m_progressDialog;
};

}           // namespace Farman

#endif // VIEWERBASE_H
