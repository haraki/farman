#include <QTextCodec>
#include <QDebug>
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

    m_textViewerEncodeList = Settings::getInstance()->getTextViewerEncodeList();
    ui->encodeComboBox->addItems(m_textViewerEncodeList);

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
    QTextCodec* codec = QTextCodec::codecForName(ui->encodeComboBox->currentText().toUtf8());

    ui->textPlainTextView->setPlainText(codec->toUnicode(m_buffer));

    return 0;
}

}
