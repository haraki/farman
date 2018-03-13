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

    ui->textPlainTextEdit->setFocus();
}

TextViewer::~TextViewer()
{
    delete ui;
}

void TextViewer::initFont()
{
    ui->textPlainTextEdit->setFont(Settings::getInstance()->getFontSetting("textViewer"));
}

void TextViewer::initPalette()
{
    QPalette pal;

    pal = ui->textPlainTextEdit->palette();
    pal.setColor(QPalette::Text, Settings::getInstance()->getColorSetting("textViewer_text"));
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("textViewer_background"));
    ui->textPlainTextEdit->setAutoFillBackground(true);
    ui->textPlainTextEdit->setPalette(pal);
}

int TextViewer::setData()
{
    ui->textPlainTextEdit->setPlainText(m_buffer);

    return 0;
}

}
