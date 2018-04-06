#ifndef HEXVIEWER_H
#define HEXVIEWER_H

#include "viewerbase.h"

namespace Ui {
class HexViewer;
}

namespace Farman
{

class HexViewer : public ViewerBase
{
    Q_OBJECT

public:
    explicit HexViewer(const QString& filePath, QWidget *parent = Q_NULLPTR);
    ~HexViewer();

private:
    int setData() Q_DECL_OVERRIDE;

    Ui::HexViewer *ui;
};

}           // namespace Farman

#endif // HEXVIEWER_H
