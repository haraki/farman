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
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), getLineNumberAreaWidth(), cr.height()));
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

void PlainTextView::lineNumberAreaPaintEvent(QPaintEvent *e)
{
    if(!m_lineNumberArea->isVisible())
    {
        return;
    }

    QPainter painter(m_lineNumberArea);

    painter.fillRect(e->rect(), m_lineNumberArea->palette().base());

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber() + 1;
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();
    int width = m_lineNumberArea->width();
    int height = fontMetrics().height();

    painter.setPen(m_lineNumberArea->palette().text().color());

    while(block.isValid() && top <= e->rect().bottom())
    {
        if (block.isVisible() && bottom >= e->rect().top())
        {
            QString number = QString::number(blockNumber);
            painter.drawText(0, top, width, height, Qt::AlignRight, number);
        }

        block = block.next();

        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();

        blockNumber++;
    }
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
    setViewportMargins(getLineNumberAreaWidth(), 0, 0, 0);
}

int PlainTextView::getLineNumberAreaWidth()
{
    if(!m_lineNumberArea->isVisible())
    {
        return 0;
    }

    int digits = getDigitsNumber();
    if(digits < 2)
    {
        digits = 2;
    }

    return fontMetrics().width('8') * (digits + 2);
}

int PlainTextView::getDigitsNumber()
{
    int lineNum = blockCount();

    return log10(lineNum) + 1;
}

PlainTextView::LineNumberArea::LineNumberArea(PlainTextView* parent) :
    QWidget(parent),
    m_parent(parent)
{
}

PlainTextView::LineNumberArea::~LineNumberArea()
{
}

QSize PlainTextView::LineNumberArea::sizeHint() const
{
    return QSize(m_parent->getLineNumberAreaWidth(), 0);
}

void PlainTextView::LineNumberArea::paintEvent(QPaintEvent* e)
{
    m_parent->lineNumberAreaPaintEvent(e);
}

}           // namespace Farman
