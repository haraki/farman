#include <math.h>
#include <QWidget>
#include <QTextBlock>
#include <QPainter>
#include <QDebug>
#include "plaintextview.h"

namespace Farman
{

PlainTextView::PlainTextView(QWidget *parent/* = Q_NULLPTR*/) :
    QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(onBlockCountChanged(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(onUpdateRequest(QRect,int)));

    updateLineNumberAreaWidth();
}

PlainTextView::~PlainTextView()
{

}

void PlainTextView::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), m_lineNumberArea->getAreaWidth(), cr.height()));
}

bool PlainTextView::getVisibleLineNumberArea()
{
    return m_lineNumberArea->isVisible();
}

void PlainTextView::setVisibleLineNumberArea(bool visible)
{
    m_lineNumberArea->setVisible(visible);

    updateLineNumberAreaWidth();
}

const QPalette& PlainTextView::getLineNumberAreaPalette() const
{
    return m_lineNumberArea->palette();
}

void PlainTextView::setLineNumberAreaPalette(const QPalette& palette)
{
    m_lineNumberArea->setPalette(palette);
}

void PlainTextView::onBlockCountChanged(int newBlockCount)
{
    Q_UNUSED(newBlockCount);

    updateLineNumberAreaWidth();
}

void PlainTextView::onUpdateRequest(const QRect &rect, int dy)
{
    if(dy != 0)
    {
        m_lineNumberArea->scroll(0, dy);
    }
    else
    {
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
    }

    if(rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth();
    }
}

void PlainTextView::updateLineNumberAreaWidth()
{
    setViewportMargins(m_lineNumberArea->getAreaWidth(), 0, 0, 0);
}

int PlainTextView::getDigitsLineNumber()
{
    int lineNum = blockCount();

    return log10(lineNum) + 1;
}

PlainTextView::LineNumberArea::LineNumberArea(PlainTextView* parent) :
    QWidget(parent)
{
}

PlainTextView::LineNumberArea::~LineNumberArea()
{
}

int PlainTextView::LineNumberArea::getAreaWidth() const
{
    if(!isVisible())
    {
        return 0;
    }

    PlainTextView* parent = dynamic_cast<PlainTextView*>(this->parent());

    Q_ASSERT(parent);

    int digits = parent->getDigitsLineNumber();
    if(digits < 2)
    {
        digits = 2;
    }

    return parent->fontMetrics().width('8') * (digits + 2);
}

QSize PlainTextView::LineNumberArea::sizeHint() const
{
    return QSize(getAreaWidth(), 0);
}

void PlainTextView::LineNumberArea::paintEvent(QPaintEvent* e)
{
    if(!isVisible())
    {
        return;
    }

    QPainter painter(this);
    PlainTextView* parent = dynamic_cast<PlainTextView*>(this->parent());

    Q_ASSERT(parent);

    painter.fillRect(e->rect(), this->palette().base());

    QTextBlock block = parent->firstVisibleBlock();
    int top = parent->blockBoundingGeometry(block).translated(parent->contentOffset()).top();
    int bottom = top + parent->blockBoundingRect(block).height();
    int width = this->width();
    int height = parent->fontMetrics().height();

    painter.setPen(this->palette().text().color());

    while(block.isValid() && top <= e->rect().bottom())
    {
        if (block.isVisible() && bottom >= e->rect().top())
        {
            QString number = QString::number(block.blockNumber() + 1);
            painter.drawText(0, top, width, height, Qt::AlignRight, number);
        }

        block = block.next();

        top = bottom;
        bottom = top + parent->blockBoundingRect(block).height();
    }
}

}           // namespace Farman
