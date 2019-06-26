#ifndef WIN32_H
#define WIN32_H

#include <QtGlobal>

#ifdef Q_OS_WIN

#include <QFileInfo>
#include <QString>

namespace Farman
{

namespace Win32
{

extern bool isSystemFile(const QFileInfo& fileInfo);
extern qint64 getFileSizeOnDisk(const QString& filePath);

}           // namespace Win32

}           // namespace Farman

using namespace Farman::Win32;

#endif

#endif // WIN32_H
