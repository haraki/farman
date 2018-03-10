#ifndef VIEWERDISPATCHER_H
#define VIEWERDISPATCHER_H

class QWidget;
class QString;
class QMimeDatabase;

namespace Farman
{

class ViewerBase;

class ViewerDispatcher
{
public:
    static void create();
    static ViewerDispatcher* getInstance();

    ViewerBase* dispatcher(const QString& filePath, QWidget* parent = Q_NULLPTR);

private:
    explicit ViewerDispatcher();
    ~ViewerDispatcher();

    static ViewerDispatcher* s_instance;

    QMimeDatabase* m_mimeDb;
};

}

#endif // VIEWERDISPATCHER_H
