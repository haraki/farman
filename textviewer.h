#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include "viewerbase.h"

namespace Ui {
class TextViewer;
}

namespace Farman
{

class TextViewer : public ViewerBase
{
    Q_OBJECT

public:
    explicit TextViewer(const QString& filePath, QWidget *parent = Q_NULLPTR);
    ~TextViewer();

private:
    void initFont();
    void initPalette();
    int setData() Q_DECL_OVERRIDE;

    Ui::TextViewer *ui;
};

}           // namespace Farman

#endif // TEXTVIEWER_H
