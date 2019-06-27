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

extern QString getCompilerVersion()
{
#if defined(__clang__)
    return QString("Clang %1").arg(__clang_version__)
#elif defined(__GNUC__)
    return QString("gcc %1.%2.%3").arg(__GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__)
#else
    return QString("unknown")
#endif
            ;
}

}           // namespace Xnix

}           // namespace Farman

#endif



