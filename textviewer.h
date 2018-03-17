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

private Q_SLOTS:
    void on_encodeComboBox_activated(int index);
    void on_showLineNumberCheckBox_stateChanged(int arg1);
    void on_wordWrapCheckBox_stateChanged(int arg1);

private:
    void initFont();
    void initPalette();
    int setData() Q_DECL_OVERRIDE;

    Ui::TextViewer *ui;

    QList<QString> m_textViewerEncodeList;
};

}           // namespace Farman

#endif // TEXTVIEWER_H
