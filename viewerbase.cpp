#include <QKeyEvent>
#include <QByteArray>
#include <QDebug>
#include <QProgressDialog>
#include "viewerbase.h"
#include "mainwindow.h"
#include "readfileworker.h"

namespace Farman
{

ViewerBase::ViewerBase(const QString& filePath, QWidget *parent/* = Q_NULLPTR*/) :
    QWidget(parent),
    m_filePath(filePath),
    m_buffer(),
    m_worker(Q_NULLPTR),
    m_progressDialog(Q_NULLPTR)
{
    connect(this,
            SIGNAL(closeViewer(const QString)),
            MainWindow::getInstance(),
            SLOT(onCloseViewer(const QString)));
}

ViewerBase::~ViewerBase()
{
    qDebug() << "~ViewerBase()";

    delete m_progressDialog;
}

int ViewerBase::start()
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
            SIGNAL(outputConsole(const QString)),
            MainWindow::getInstance(),
            SLOT(onOutputConsole(const QString)));
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

bool ViewerBase::eventFilter(QObject *watched, QEvent *e)
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
                emitCloseViewer();

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

void ViewerBase::onReadFileFinished(int result)
{
    qDebug() << "ViewerBase::onReadFileFinished : result : " << result;

    m_worker = Q_NULLPTR;           // deleteLater で自動的にデストラクトされているので、Q_NULLPTR を代入するのみ

    if(result == 0)
    {
        if(setData() < 0)
        {
            emitCloseViewer();
        }
    }
    else
    {
        emitCloseViewer();
    }
}

void ViewerBase::onReadFileError(const QString& err)
{
    qDebug() << "ViewerBase::onReadFileError : err : " << err;
}

void ViewerBase::onProgressDialogCanceled()
{
    qDebug() << "ViewerBase::onProgressDialogCanceled()";

    m_worker->abort();
}

void ViewerBase::emitCloseViewer()
{
    emit closeViewer(objectName());
}

}           // namespace Farman
