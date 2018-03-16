#include "textviewer.h"
#include "ui_textviewer.h"
#include "plaintextview.h"
#include "settings.h"

namespace Farman
{

TextViewer::TextViewer(const QString& filePath, QWidget *parent/* = Q_NULLPTR*/) :
    ViewerBase(filePath, parent),
    ui(new Ui::TextViewer)
{
    ui->setupUi(this);

    ui->showLineNumberCheckBox->setChecked(Settings::getInstance()->getTextViewerShowLineNumber());
    ui->wordWrapCheckBox->setChecked(Settings::getInstance()->getTextViewerWordWrap());

    ui->textPlainTextView->setVisibleLineNumberArea(ui->showLineNumberCheckBox->isChecked());
    ui->textPlainTextView->setLineWrapMode(ui->wordWrapCheckBox->isChecked() ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);

    initFont();

    initPalette();

    ui->textPlainTextView->setFocus();
}

TextViewer::~TextViewer()
{
    delete ui;
}

void TextViewer::on_showLineNumberCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    Settings::getInstance()->setTextViewerShowLineNumber(ui->showLineNumberCheckBox->isChecked());
    ui->textPlainTextView->setVisibleLineNumberArea(ui->showLineNumberCheckBox->isChecked());
}

void TextViewer::on_wordWrapCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    Settings::getInstance()->setTextViewerWordWrap(ui->wordWrapCheckBox->isChecked());
    ui->textPlainTextView->setLineWrapMode(ui->wordWrapCheckBox->isChecked() ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
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
