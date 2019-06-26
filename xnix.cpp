#include <QtGlobal>

#ifndef Q_OS_WIN

#include <sys/stat.h>
#include <QFileInfo>
#include <QDebug>
#include "xnix.h"

namespace Farman
{

namespace Xnix
{

qint64 getFileSizeOnDisk(const QString& filePath)
{
    struct stat statBuf;

    if(lstat(filePath.toUtf8(), &statBuf) == -1)
    {
        return -1;
    }

    qDebug() << "st_size : " << statBuf.st_size << ", st_blocks : " << statBuf.st_blocks;

    return statBuf.st_blocks * 512;
}

}           // namespace Xnix

}           // namespace Farman

#endif



