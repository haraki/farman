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
    return QString("gcc %1, %2").arg(__VERSION__)
#   if defined(__x86_64__)
            .arg("64bit")
#   else
            .arg("32bit")
#   endif
#else
    return QString("")
#endif
            ;
}

}           // namespace Xnix

}           // namespace Farman

#endif



