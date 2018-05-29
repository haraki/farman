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
    static void create();
    static ViewerDispatcher* getInstance();

    int initialize();

    ViewerBase* dispatcher(const QString& filePath, ViewerType viewerType, QWidget* parent = Q_NULLPTR);

private:
    explicit ViewerDispatcher();
    ~ViewerDispatcher();

    static ViewerDispatcher* s_instance;

    QMap<QString, ViewerType> m_viewerTypeSettings;
};

}

#endif // VIEWERDISPATCHER_H
