#include <QMimeDatabase>
#include <QImageReader>
#include <QFileInfo>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsScene>
#include "viewerdispatcher.h"
#include "imageviewer.h"

namespace Farman
{

ViewerDispatcher* ViewerDispatcher::s_instance = Q_NULLPTR;

void ViewerDispatcher::create()
{
    s_instance = new ViewerDispatcher();
}

ViewerDispatcher* ViewerDispatcher::getInstance()
{
    return s_instance;
}

ViewerDispatcher::ViewerDispatcher() :
    m_mimeDb(new QMimeDatabase())
{
    for(auto mimeType : QMimeDatabase().allMimeTypes())
    {
        qDebug() << mimeType.name() << ", " << mimeType.suffixes();
    }

    qDebug() << "----------------------------------";

    for(auto format : QImageReader::supportedMimeTypes())
    {
        qDebug() << format;
    }
}

ViewerDispatcher::~ViewerDispatcher()
{
    delete m_mimeDb;
}

QWidget* ViewerDispatcher::dispatcher(const QString& filePath, QWidget* parent/* = Q_NULLPTR*/)
{
    QMimeType mimeType = m_mimeDb->mimeTypeForFile(filePath);
    QByteArray mimeTypeName = mimeType.name().toUtf8();
    qDebug() << "mimeType : " << mimeType.name();

    if(QImageReader::supportedMimeTypes().indexOf(mimeTypeName) >= 0)
    {
        qDebug() << "create Image viewer.";

        return new ImageViewer(filePath, parent);
    }

    return Q_NULLPTR;
}

}
