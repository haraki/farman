#include <QtGlobal>

#ifdef Q_OS_WIN

#include <Windows.h>
#include <QFileInfo>
#include <QStorageInfo>
#include <QDebug>
#include "win32.h"
#include "misc.h"

namespace Farman
{

namespace Win32
{

bool isSystemFile(const QFileInfo& fileInfo)
{
    DWORD attrFlags = ::GetFileAttributes(fileInfo.filePath().toStdWString().c_str());
    if(attrFlags != static_cast<DWORD>(-1) && (attrFlags & FILE_ATTRIBUTE_SYSTEM))
    {
        return true;
    }

    return false;
}

qint64 getFileSizeOnDisk(const QString& filePath)
{
    DWORD sectorsPerCluster = 0;
    DWORD bytesPerSector = 0;
    DWORD freeClusters = 0;
    DWORD totalClusters = 0;

    if(!::GetDiskFreeSpace(QStorageInfo(filePath).rootPath().toStdWString().c_str(),
                           &sectorsPerCluster,
                           &bytesPerSector,
                           &freeClusters,
                           &totalClusters))
    {
        qDebug() << "GetDiskFreeSpace() error : " << ::GetLastError() << endl;
        return -1;
    }

    DWORD clusterSize = sectorsPerCluster * bytesPerSector;

    DWORD fileSizeH, fileSizeL;
    fileSizeL = ::GetCompressedFileSize(filePath.toStdWString().c_str(), &fileSizeH);
    if(fileSizeL == static_cast<DWORD>(-1))
    {
        return -1;
    }

    qint64 sizeOnDisk = RoundUp((static_cast<qint64>(fileSizeH) << 32) | fileSizeL, clusterSize);

    qDebug() << "clusterSize : " << clusterSize << ", fileSizeH : " << fileSizeH << ", fileSizeL : " << fileSizeL << ", sizeOnDisk : " << sizeOnDisk;

    return sizeOnDisk;
}

QString getCompilerVersion()
{
#ifdef _MSC_VER
    return QString("MSVC %1(%2), %3")
            .arg(
#   if _MSC_VER >= 1920
                "2019"
#   elif _MSC_VER >= 1910
                "2017"
#   elif _MSC_VER >= 1900
                "2015"
#   elif _MSC_VER >= 1800
                "2013"
#   elif _MSC_VER >= 1700
                "2012"
#   elif _MSC_VER >= 1600
                "2010"
#   elif _MSC_VER >= 1500
                "2008"
#   elif _MSC_VER >= 1400
                "2005"
#   elif _MSC_VER >= 1310
                "2003"
#   elif _MSC_VER >= 1300
                "2002"
#   elif _MSC_VER >= 1200
                "6.0"
#   else
                "unknown"
#   endif
                )
            .arg(_MSC_FULL_VER)
            .arg(
#   if defined(_WIN64)
                "64bit"
#   else
                "32bit"
#   endif
                )
#elif defined(__MINGW64__)
    return QString("MinGW %1, 64bit").arg(__MINGW64_VERSION_STR)
#elif defined(__MINGW32__)
    return QString("MinGW %1.%2, 32bit").arg(__MINGW32_MAJOR_VERSION).arg(__MINGW32_MINOR_VERSION)
#else
    return QString("")
#endif
                ;
}

}           // namespace Win32

}           // namespace Farman

#endif
