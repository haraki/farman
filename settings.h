#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QTextCodec>
#include <QPoint>
#include <QSize>
#include <QColor>
#include <QFont>
#include <QPair>
#include "types.h"

namespace Farman
{

class Settings : public QSettings
{
    Q_OBJECT

public:
    static void create();
    static Settings* getInstance();

    void flush();

    SizeAtStartup getSizeAtStartupType() { return m_sizeAtStartupType; }
    void setSizeAtStartupType(SizeAtStartup sizeAtStartupType) { m_sizeAtStartupType = sizeAtStartupType; }

    QSize getSizeAtStartup() { return m_sizeAtStartup; }
    void setSizeAtStartup(const QSize& sizeAtStartup) { m_sizeAtStartup = sizeAtStartup; }

    PositionAtStartup getPositionAtStartupType() { return m_positionAtStartupType; }
    void setPositionAtStartupType(PositionAtStartup positionAtStartupType) { m_positionAtStartupType = positionAtStartupType; }

    QPoint getPositionAtStartup() { return m_positionAtStartup; }
    void setPositionAtStartup(const QPoint& positionAtStartup) { m_positionAtStartup = positionAtStartup; }

    ViewMode getViewMode() { return m_viewMode; }
    void setViewMode(ViewMode viewMode) { m_viewMode = viewMode; }

    FolderAtStartup getLeftFolderAtStartup() { return m_leftFolderAtStartup; }
    void setLeftFolderAtStartup(FolderAtStartup leftFolderAtStartup) { m_leftFolderAtStartup = leftFolderAtStartup; }

    QString getLeftFolderPath() { return m_leftFolderPath; }
    void setLeftFolderPath(const QString& leftFolderPath) { m_leftFolderPath = leftFolderPath; }

    FolderAtStartup getRightFolderAtStartup() { return m_rightFolderAtStartup; }
    void setRightFolderAtStartup(FolderAtStartup rightFolderAtStartup) { m_rightFolderAtStartup = rightFolderAtStartup; }

    QString getRightFolderPath() { return m_rightFolderPath; }
    void setRightFolderPath(const QString& rightFolderPath) { m_rightFolderPath = rightFolderPath; }

    QDir::SortFlags getLeftSortSettings() { return m_leftSortSettings; }
    void setLeftSortSettings(QDir::SortFlags leftSortSettings) { m_leftSortSettings = leftSortSettings; }

    QDir::SortFlags getRightSortSettings() { return m_rightSortSettings; }
    void setRightSortSettings(QDir::SortFlags rightSortSettings) { m_rightSortSettings = rightSortSettings; }

    QDir::Filters getLeftFilterSettings() { return m_leftFilterSettings; }
    void setLeftFilterSettings(QDir::Filters leftFilterSettings) { m_leftFilterSettings = leftFilterSettings; }

    QDir::Filters getRightFilterSettings() { return m_rightFilterSettings; }
    void setRightFilterSettings(QDir::Filters rightFilterSettings) { m_rightFilterSettings = rightFilterSettings; }

    bool getConfirmQuit() { return m_confirmQuit; }
    void setConfirmQuit(bool confirmQuit) { m_confirmQuit = confirmQuit; }

    QColor getColorSetting(const QString& colorSettingType);
    void setColorSetting(const QString& colorSettingType, const QColor& color);

    const QMap<QString, QColor>& getColorSettings() { return m_colorSettings; }
    void setColorSettings(const QMap<QString, QColor>& settings) { m_colorSettings = settings; }

    QFont getFontSetting(const QString& fontSettingType);
    void setFontSetting(const QString& fontSettingType, const QFont& font);

    const QMap<QString, QFont>& getFontSettings() { return m_fontSettings; }
    void setFontSettings(const QMap<QString, QFont>& settings) { m_fontSettings = settings; }

    int getCursorWidth() { return m_cursorWidth; }
    void setCursorWidth(int cursorWidth) { m_cursorWidth = cursorWidth; }

    bool getImageViewerFitInView() { return m_imageViewerFitInView; }
    void setImageViewerFitInView(bool fitInView) { m_imageViewerFitInView = fitInView; }

    ImageViewerBGType getImageViewerBGType() { return m_imageViewerBGType; }
    void setImageViewerBGType(ImageViewerBGType bgType) { m_imageViewerBGType = bgType; }

    bool getTextViewerShowLineNumber() { return m_textViewerShowLineNumber; }
    void setTextViewerShowLineNumber(bool show) { m_textViewerShowLineNumber = show; }

    bool getTextViewerWordWrap() { return m_textViewerWordWrap; }
    void setTextViewerWordWrap(bool enabled) { m_textViewerWordWrap = enabled; }

    const QList<QString>& getTextViewerEncodeList() { return m_textViewerEncodeList; }
    void setTextViewerEncodeList(const QList<QString>& encodeList) { m_textViewerEncodeList = encodeList; }

private:
    Settings();

    void initialize();

    QDir::SortFlags getSortSettings(const QString& prefix);
    void setSortSettings(QDir::SortFlags sortSettings, const QString& prefix);

    QDir::Filters getFilterSettings(const QString& prefix);
    void setFilterSettings(QDir::Filters filterSettings, const QString& prefix);

    static Settings* s_instance;

    SizeAtStartup m_sizeAtStartupType = SizeAtStartup::Default;
    QSize m_sizeAtStartup = QSize(0, 0);

    PositionAtStartup m_positionAtStartupType = PositionAtStartup::Default;
    QPoint m_positionAtStartup = QPoint(0, 0);

    ViewMode m_viewMode = ViewMode::Double;

    FolderAtStartup m_leftFolderAtStartup = FolderAtStartup::Default;
    QString m_leftFolderPath = "";
    FolderAtStartup m_rightFolderAtStartup = FolderAtStartup::Default;
    QString m_rightFolderPath = "";

    QDir::SortFlags m_leftSortSettings = DEFAULT_SORT_FLAGS;
    QDir::SortFlags m_rightSortSettings = DEFAULT_SORT_FLAGS;

    QDir::Filters m_leftFilterSettings = DEFAULT_FILTER_FLAGS;
    QDir::Filters m_rightFilterSettings = DEFAULT_FILTER_FLAGS;

    bool m_confirmQuit = true;

    const QMap<QString, QColor> m_defaultColorSettings =
    {
        { "folderView_normal",              "#000000", },
        { "folderView_normal_selected",     "#000000", },
        { "folderView_folder",              "#0000ff", },
        { "folderView_folder_selected",     "#0000ff", },
        { "folderView_readOnly",            "#00ff00", },
        { "folderView_readOnly_selected",   "#00ff00", },
        { "folderView_hidden",              "#888888", },
        { "folderView_hidden_selected",     "#888888", },
        { "folderView_system",              "#ff0000", },
        { "folderView_system_selected",     "#ff0000", },

        { "folderView_background",          "#ffffff", },
        { "folderView_selected_background", "#b2d7ff", },

        { "folderView_cursor",              "#0000ff", },
        { "folderView_cursor_inactive",     "#cccccc", },

        { "folderPath_text",                "#000000", },
        { "folderPath_background",          "#ffffff", },

        { "console_text",                   "#000000", },
        { "console_background",             "#ffffff", },

        { "imageViewer_background",         "#808080", },

        { "textViewer_text",                  "#000000", },
        { "textViewer_background",            "#ffffff", },
        { "textViewer_lineNumber_text",       "#000000", },
        { "textViewer_lineNumber_background", "#808080", },
    };

    QMap<QString, QColor> m_colorSettings = m_defaultColorSettings;

    static const QFont getDefaultFixedFont()
    {
#if defined(Q_OS_WIN)
        return QFont("Terminal", 14);
#elif defined(Q_OS_MAC)
        return QFont("Monaco", 13);
#else
        return QFont();
#endif
    }

    const QMap<QString, QFont> m_defaultFontSettings =
    {
        { "folderView", getDefaultFixedFont(), },
        { "folderPath", getDefaultFixedFont(), },
        { "console",    getDefaultFixedFont(), },
        { "textViewer", getDefaultFixedFont(), },
    };

    QMap<QString, QFont> m_fontSettings = m_defaultFontSettings;

    int m_cursorWidth = DEFAULT_CURSOR_WIDTH;

    bool m_imageViewerFitInView = true;
    ImageViewerBGType m_imageViewerBGType = ImageViewerBGType::Checkered;

    bool m_textViewerShowLineNumber = true;
    bool m_textViewerWordWrap = false;

    QList<QString> m_textViewerEncodeList;
};

}

#endif // SETTINGS_H
