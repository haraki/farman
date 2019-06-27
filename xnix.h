#ifndef XNIX_H
#define XNIX_H

#include <QtGlobal>

#ifndef Q_OS_WIN

#include <QString>

namespace Farman
{

namespace Xnix
{

extern qint64 getFileSizeOnDisk(const QString& filePath);
extern QString getCompilerVersion();

}           // namespace Xnix

}           // namespace Farman

using namespace Farman::Xnix;

#endif

#endif // XNIX_H
