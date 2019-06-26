#ifndef MISC_H
#define MISC_H

#include <QtGlobal>

namespace Farman
{

template <class T> T RoundUp(T n, quint64 m)
{
    return (n / m) * m + (((n % m) != 0) ? m : 0);
}

}           // namespace Farman

#endif // MISC_H
