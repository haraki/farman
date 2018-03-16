#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QWidget>
#include <QPlainTextEdit>

namespace Farman
{
/*
 * Reference
 *  Qt Code Editor Example
 *  http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 */
class PlainTextView : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit PlainTextView(QWidget *parent = Q_NULLPTR);
    ~PlainTextView();

    bool getVisibleLineNumberArea();
    void setVisibleLineNumberArea(bool visible);

    const QPalette& getLineNumberAreaPalette() const;
    void setLineNumberAreaPalette(const QPalette& palette);
    void lineNumberAreaPaintEvent(QPaintEvent *e);

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onBlockCountChanged(int newBlockCount);
    void onUpdateRequest(const QRect &rect, int dy);

private:
    void updateLineNumberAreaWidth();
    int getLineNumberAreaWidth();
    int getDigitsNumber();

    class LineNumberArea : public QWidget
    {
    public:
        explicit LineNumberArea(PlainTextView* parent);
        ~LineNumberArea();

        QSize sizeHint() const Q_DECL_OVERRIDE;

    protected:
        void paintEvent(QPaintEvent* e) Q_DECL_OVERRIDE;

    private:
        PlainTextView* m_parent;
    };

    LineNumberArea* m_lineNumberArea;
};

}           // namespace Farman

#endif // PLAINTEXTEDIT_H
