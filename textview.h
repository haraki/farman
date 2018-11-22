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
class TextView : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit TextView(QWidget *parent = Q_NULLPTR);
    ~TextView() Q_DECL_OVERRIDE;

    bool getVisibleLineNumberArea();
    void setVisibleLineNumberArea(bool visible);

    const QPalette& getLineNumberAreaPalette() const;
    void setLineNumberAreaPalette(const QPalette& palette);

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onBlockCountChanged(int newBlockCount);
    void onUpdateRequest(const QRect &rect, int dy);

private:
    void updateLineNumberAreaWidth();
    int getLineNumberAreaWidth();
    int getDigitsLineNumber();

    class LineNumberArea : public QWidget
    {
    public:
        explicit LineNumberArea(TextView* parent);
        ~LineNumberArea();

        int getAreaWidth() const;

    private:
        QSize sizeHint() const Q_DECL_OVERRIDE;

        void paintEvent(QPaintEvent* e) Q_DECL_OVERRIDE;
    };

    LineNumberArea* m_lineNumberArea;
};

}           // namespace Farman

#endif // PLAINTEXTEDIT_H
