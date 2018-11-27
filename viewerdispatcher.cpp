#include <QMimeDatabase>
#include <QImageReader>
#include <QFileInfo>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsScene>
#include "viewerdispatcher.h"
#include "settings.h"
#include "imageviewer.h"
#include "textviewer.h"
#include "hexviewer.h"
#include "types.h"

namespace Farman
{

ViewerDispatcher::ViewerDispatcher(QWidget* parent)
    : m_parent(parent)
{
}

ViewerDispatcher::~ViewerDispatcher()
{
}

int ViewerDispatcher::initialize()
{
    m_viewerTypeSettings.clear();

    Settings::getInstance()->beginGroup("mime");

    QStringList mimeKeys = Settings::getInstance()->allKeys();

    qDebug() << mimeKeys;

    if(mimeKeys.size() == 0)
    {
        static const QList<QString> textMimeTypes =
        {
            "application/xml",
            "application/x-perl",
            "application/x-ruby",
            "application/javascript",
            "application/json",
            "application/x-shellscript",
        };

        for(auto mime : textMimeTypes)
        {
            m_viewerTypeSettings[mime] = ViewerType::Text;
        }
    }
    else
    {
        for(auto mimeKey : mimeKeys)
        {
            QString viewerTypeValue = Settings::getInstance()->value(mimeKey, "hex").toString();
            m_viewerTypeSettings[mimeKey] = (viewerTypeValue == "image") ? ViewerType::Image :
                                            (viewerTypeValue == "text") ? ViewerType::Text :
                                                                          ViewerType::Hex;
        }
    }

    Settings::getInstance()->endGroup();

    qDebug() << "====================================";
    for(auto key : m_viewerTypeSettings.keys())
    {
        qDebug() << key << ":" << ((m_viewerTypeSettings[key] == ViewerType::Image) ? "image" :
                                   (m_viewerTypeSettings[key] == ViewerType::Text) ? "text" :
                                                                                     "hex");
    }

    return 0;
}

ViewerBase* ViewerDispatcher::dispatcher(const QString& filePath, ViewerType viewerType)
{
    QMimeType mimeType = QMimeDatabase().mimeTypeForFile(filePath);
    qDebug() << "mimeType : " << mimeType.name();

    if(viewerType == ViewerType::Auto)
    {
        // mimeType に紐付けられている Viewer があればそれを起動する
        auto viewerTypeItr = m_viewerTypeSettings.find(mimeType.name());
        if(viewerTypeItr != m_viewerTypeSettings.end())
        {
            viewerType = *viewerTypeItr;
        }
    }

    if(viewerType == ViewerType::Auto)
    {
        if(QImageReader::supportedMimeTypes().indexOf(mimeType.name().toUtf8()) >= 0)
        {
            // QImageReader がサポートしているフォーマットの場合は ImageViewer を起動する
            viewerType = ViewerType::Image;
        }
        else if(mimeType.name().startsWith("text/"))
        {
            // MIME-type が "text/〜" の場合は TextViewer を起動する
            viewerType = ViewerType::Text;
        }
        else
        {
            // 上記いずれの条件にも該当しない場合は HexViewer を起動する
            viewerType = ViewerType::Hex;
        }
    }

    if(viewerType == ViewerType::Image)
    {
        qDebug() << "create Image viewer.";

        return new ImageViewer(filePath, m_parent);
    }
    else if(viewerType == ViewerType::Text)
    {
        qDebug() << "create Text viewer.";

        return new TextViewer(filePath, m_parent);
    }
    else if(viewerType == ViewerType::Hex)
    {
        qDebug() << "create Hex viewer.";

        return new HexViewer(filePath, m_parent);
    }

    return Q_NULLPTR;
}

}
