#include "textviewer.h"
#include "ui_textviewer.h"
#include "settings.h"

namespace Farman
{

TextViewer::TextViewer(const QString& filePath, QWidget *parent/* = Q_NULLPTR*/) :
    ViewerBase(filePath, parent),
    ui(new Ui::TextViewer)
{
    ui->setupUi(this);

    initFont();

    initPalette();

    ui->textPlainTextView->setFocus();
}

TextViewer::~TextViewer()
{
    delete ui;
}

void TextViewer::initFont()
{
    ui->textPlainTextView->setFont(Settings::getInstance()->getFontSetting("textViewer"));
}

void TextViewer::initPalette()
{
    QPalette pal;

    pal = ui->textPlainTextView->palette();
    pal.setColor(QPalette::Text, Settings::getInstance()->getColorSetting("textViewer_text"));
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("textViewer_background"));
    ui->textPlainTextView->setPalette(pal);

    pal = ui->textPlainTextView->getLineNumberAreaPalette();
    pal.setColor(QPalette::Text, Settings::getInstance()->getColorSetting("textViewer_lineNumber_text"));
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("textViewer_lineNumber_background"));
    ui->textPlainTextView->setLineNumberAreaPalette(pal);
}

int TextViewer::setData()
{
    ui->textPlainTextView->setPlainText(m_buffer);

    return 0;
}

}
