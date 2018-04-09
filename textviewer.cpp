#include <QTextCodec>
#include <QDebug>
#include "textviewer.h"
#include "ui_textviewer.h"
#include "textview.h"
#include "settings.h"

namespace Farman
{

TextViewer::TextViewer(const QString& filePath, QWidget *parent/* = Q_NULLPTR*/) :
    ViewerBase(filePath, parent),
    ui(new Ui::TextViewer)
{
    ui->setupUi(this);

    m_textViewerEncodeList = Settings::getInstance()->getTextViewerEncodeList();
    ui->encodeComboBox->addItems(m_textViewerEncodeList);

    ui->showLineNumberCheckBox->setChecked(Settings::getInstance()->getTextViewerShowLineNumber());
    ui->wordWrapCheckBox->setChecked(Settings::getInstance()->getTextViewerWordWrap());

    ui->textView->setVisibleLineNumberArea(ui->showLineNumberCheckBox->isChecked());
    ui->textView->setLineWrapMode(ui->wordWrapCheckBox->isChecked() ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);

    initFont();

    initPalette();

    ui->textView->setFocus();
}

TextViewer::~TextViewer()
{
    delete ui;
}

void TextViewer::on_encodeComboBox_activated(int index)
{
    qDebug() << "TextViewer::on_encodeComboBox_activated(" << index << ")";

    // 選択されたエンコードをリストの先頭に移動する
    QString encode = m_textViewerEncodeList[index];
    m_textViewerEncodeList.removeAt(index);
    m_textViewerEncodeList.insert(0, encode);

    ui->encodeComboBox->blockSignals(true);
    ui->encodeComboBox->clear();
    ui->encodeComboBox->addItems(m_textViewerEncodeList);
    ui->encodeComboBox->setCurrentIndex(0);
    ui->encodeComboBox->blockSignals(false);

    Settings::getInstance()->setTextViewerEncodeList(m_textViewerEncodeList);

    setData();
}

void TextViewer::on_showLineNumberCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    Settings::getInstance()->setTextViewerShowLineNumber(ui->showLineNumberCheckBox->isChecked());
    ui->textView->setVisibleLineNumberArea(ui->showLineNumberCheckBox->isChecked());
}

void TextViewer::on_wordWrapCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    Settings::getInstance()->setTextViewerWordWrap(ui->wordWrapCheckBox->isChecked());
    ui->textView->setLineWrapMode(ui->wordWrapCheckBox->isChecked() ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
}

void TextViewer::initFont()
{
    ui->textView->setFont(Settings::getInstance()->getFontSetting("textViewer"));
}

void TextViewer::initPalette()
{
    QPalette pal;

    pal = ui->textView->palette();
    pal.setColor(QPalette::Text,            Settings::getInstance()->getColorSetting("textViewer_text"));
    pal.setColor(QPalette::Base,            Settings::getInstance()->getColorSetting("textViewer_background"));
    pal.setColor(QPalette::HighlightedText, Settings::getInstance()->getColorSetting("textViewer_selected_text"));
    pal.setColor(QPalette::Highlight,       Settings::getInstance()->getColorSetting("textViewer_selected_background"));
    ui->textView->setPalette(pal);

    pal = ui->textView->getLineNumberAreaPalette();
    pal.setColor(QPalette::Text, Settings::getInstance()->getColorSetting("textViewer_lineNumber_text"));
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("textViewer_lineNumber_background"));
    ui->textView->setLineNumberAreaPalette(pal);
}

int TextViewer::setData()
{
    QTextCodec* codec = QTextCodec::codecForName(ui->encodeComboBox->currentText().toUtf8());

    ui->textView->setPlainText(codec->toUnicode(m_buffer));

    return 0;
}

}           // namespace Farman
