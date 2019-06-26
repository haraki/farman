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

}           // namespace Win32

}           // namespace Farman

#endif
