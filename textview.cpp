#include <math.h>
#include <QWidget>
#include <QTextBlock>
#include <QPainter>
#include <QDebug>
#include "textview.h"

namespace Farman
{

TextView::TextView(QWidget *parent/* = Q_NULLPTR*/) :
    QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(onBlockCountChanged(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(onUpdateRequest(QRect,int)));

    updateLineNumberAreaWidth();
}

TextView::~TextView()
{

}

void TextView::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), m_lineNumberArea->getAreaWidth(), cr.height()));
}

bool TextView::getVisibleLineNumberArea()
{
    return m_lineNumberArea->isVisible();
}

void TextView::setVisibleLineNumberArea(bool visible)
{
    m_lineNumberArea->setVisible(visible);

    updateLineNumberAreaWidth();
}

const QPalette& TextView::getLineNumberAreaPalette() const
{
    return m_lineNumberArea->palette();
}

void TextView::setLineNumberAreaPalette(const QPalette& palette)
{
    m_lineNumberArea->setPalette(palette);
}

void TextView::onBlockCountChanged(int newBlockCount)
{
    Q_UNUSED(newBlockCount);

    updateLineNumberAreaWidth();
}

void TextView::onUpdateRequest(const QRect &rect, int dy)
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

void TextView::updateLineNumberAreaWidth()
{
    setViewportMargins(m_lineNumberArea->getAreaWidth(), 0, 0, 0);
}

int TextView::getDigitsLineNumber()
{
    int lineNum = blockCount();

    return log10(lineNum) + 1;
}

TextView::LineNumberArea::LineNumberArea(TextView* parent) :
    QWidget(parent)
{
}

TextView::LineNumberArea::~LineNumberArea()
{
}

int TextView::LineNumberArea::getAreaWidth() const
{
    if(!isVisible())
    {
        return 0;
    }

    TextView* parent = qobject_cast<TextView*>(this->parent());
    Q_ASSERT(parent != Q_NULLPTR);

    int digits = parent->getDigitsLineNumber();
    if(digits < 2)
    {
        digits = 2;
    }

    return parent->fontMetrics().width('8') * (digits + 2);
}

QSize TextView::LineNumberArea::sizeHint() const
{
    return QSize(getAreaWidth(), 0);
}

void TextView::LineNumberArea::paintEvent(QPaintEvent* e)
{
    if(!isVisible())
    {
        return;
    }

    QPainter painter(this);

    TextView* parent = qobject_cast<TextView*>(this->parent());
    Q_ASSERT(parent != Q_NULLPTR);

    painter.fillRect(e->rect(), this->palette().base());

    QTextBlock block = parent->firstVisibleBlock();
    int top = static_cast<int>(parent->blockBoundingGeometry(block).translated(parent->contentOffset()).top());
    int bottom = top + static_cast<int>(parent->blockBoundingRect(block).height());
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
