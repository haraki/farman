#ifndef VIEWERDISPATCHER_H
#define VIEWERDISPATCHER_H

#include <QMap>
#include "types.h"

class QWidget;
class QString;

namespace Farman
{

class ViewerBase;

class ViewerDispatcher
{
public:
    explicit ViewerDispatcher(QWidget* parent);
    ~ViewerDispatcher();

    int initialize();

    ViewerBase* dispatcher(const QString& filePath, ViewerType viewerType);

private:
    QWidget* m_parent;

    QMap<QString, ViewerType> m_viewerTypeSettings;
};

}

#endif // VIEWERDISPATCHER_H
