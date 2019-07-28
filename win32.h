#ifndef WIN32_H
#define WIN32_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <QString>

namespace Farman
{

namespace Win32
{

extern bool isSystemFile(const QString& filePath);
extern bool isArchiveFile(const QString& filePath);
extern qint64 getFileSizeOnDisk(const QString& filePath);
extern QString getCompilerVersion();

}           // namespace Win32

}           // namespace Farman

using namespace Farman::Win32;

#endif

#endif // WIN32_H
