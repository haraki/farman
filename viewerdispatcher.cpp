﻿#include <QMimeDatabase>
#include <QImageReader>
#include <QFileInfo>
#include <QDebug>
#include <QPixmap>
#include <QGraphicsScene>
#include "viewerdispatcher.h"
#include "settings.h"
#include "imageviewer.h"
#include "textviewer.h"

namespace Farman
{

ViewerDispatcher* ViewerDispatcher::s_instance = Q_NULLPTR;

void ViewerDispatcher::create()
{
    s_instance = new ViewerDispatcher();

    s_instance->initialize();
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

    for(auto format : QImageReader::supportedMimeTypes())
    {
        qDebug() << format;
    }
}

ViewerDispatcher::~ViewerDispatcher()
{
    delete m_mimeDb;
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
            QString viewerTypeValue = Settings::getInstance()->value(mimeKey, "binary").toString();
            m_viewerTypeSettings[mimeKey] = (viewerTypeValue == "image") ? ViewerType::Image :
                                            (viewerTypeValue == "text") ? ViewerType::Text :
                                                                          ViewerType::Binary;
        }
    }

    Settings::getInstance()->endGroup();

    qDebug() << "====================================";
    for(auto key : m_viewerTypeSettings.keys())
    {
        qDebug() << key << ":" << ((m_viewerTypeSettings[key] == ViewerType::Image) ? "image" :
                                   (m_viewerTypeSettings[key] == ViewerType::Text) ? "text" :
                                                                                     "binary");
    }

    return 0;
}

ViewerBase* ViewerDispatcher::dispatcher(const QString& filePath, QWidget* parent/* = Q_NULLPTR*/)
{
    QMimeType mimeType = m_mimeDb->mimeTypeForFile(filePath);
    qDebug() << "mimeType : " << mimeType.name();

    auto viewerTypeItr = m_viewerTypeSettings.find(mimeType.name());
    if(viewerTypeItr != m_viewerTypeSettings.end())
    {
        if(*viewerTypeItr == ViewerType::Image)
        {
            qDebug() << "create Image viewer.";

            return new ImageViewer(filePath, parent);
        }
        else if(*viewerTypeItr == ViewerType::Text)
        {
            qDebug() << "create Text viewer.";

            return new TextViewer(filePath, parent);
        }
#if 0
        else if(*viewerTypeItr == ViewerType::Binary)
        {
            qDebug() << "create Binary viewer.";

            return new BinaryViewer(filePath, parent);
        }
#endif
    }

    if(QImageReader::supportedMimeTypes().indexOf(mimeType.name().toUtf8()) >= 0)
    {
        // QImageReader がサポートしているフォーマットの場合は ImageViewer を起動する
        qDebug() << "create Image viewer.";

        return new ImageViewer(filePath, parent);
    }
    else if(mimeType.name().startsWith("text/"))
    {
        // MIME-type が "text/〜" の場合は TextViewer を起動する
        qDebug() << "create Text viewer.";

        return new TextViewer(filePath, parent);
    }

    return Q_NULLPTR;
}

}