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

    PaneMode getPaneMode() { return m_paneMode; }
    void setPaneMode(PaneMode paneMode) { m_paneMode = paneMode; }

    PaneType getActivePane() { return m_activePane; }
    void setActivePane(PaneType pane) { m_activePane = pane; }

    FolderAtStartup getLeftFolderAtStartup() { return m_leftFolderAtStartup; }
    void setLeftFolderAtStartup(FolderAtStartup leftFolderAtStartup) { m_leftFolderAtStartup = leftFolderAtStartup; }

    QString getLeftFolderPath() { return m_leftFolderPath; }
    void setLeftFolderPath(const QString& leftFolderPath) { m_leftFolderPath = leftFolderPath; }

    FolderAtStartup getRightFolderAtStartup() { return m_rightFolderAtStartup; }
    void setRightFolderAtStartup(FolderAtStartup rightFolderAtStartup) { m_rightFolderAtStartup = rightFolderAtStartup; }

    QString getRightFolderPath() { return m_rightFolderPath; }
    void setRightFolderPath(const QString& rightFolderPath) { m_rightFolderPath = rightFolderPath; }

    SectionType getLeftSortSectionType() { return m_leftSortSectionType; }
    SortDirsType getLeftSortDirsType() { return m_leftSortDirsType; }
    bool getLeftSortDotFirst() { return m_leftSortDotFirst; }
    Qt::CaseSensitivity getLeftSortCaseSensitivity() { return m_leftSortCaseSensitivity; }
    Qt::SortOrder getLeftSortOrder() { return m_leftSortOrder; }
    void setLeftSortSectionType(SectionType sectionType) { m_leftSortSectionType = sectionType; }
    void setLeftSortDirsType(SortDirsType dirsType) { m_leftSortDirsType = dirsType; }
    void setLeftSortDotFirst(bool dotFirst) { m_leftSortDotFirst = dotFirst; }
    void setLeftSortCaseSensitivity(Qt::CaseSensitivity caseSensitivity) { m_leftSortCaseSensitivity = caseSensitivity; }
    void setLeftSortOrder(Qt::SortOrder order) { m_leftSortOrder = order; }

    SectionType getRightSortSectionType() { return m_rightSortSectionType; }
    SortDirsType getRightSortDirsType() { return m_rightSortDirsType; }
    bool getRightSortDotFirst() { return m_rightSortDotFirst; }
    Qt::CaseSensitivity getRightSortCaseSensitivity() { return m_rightSortCaseSensitivity; }
    Qt::SortOrder getRightSortOrder() { return m_rightSortOrder; }
    void setRightSortSectionType(SectionType sectionType) { m_rightSortSectionType = sectionType; }
    void setRightSortDirsType(SortDirsType dirsType) { m_rightSortDirsType = dirsType; }
    void setRightSortDotFirst(bool dotFirst) { m_rightSortDotFirst = dotFirst; }
    void setRightSortCaseSensitivity(Qt::CaseSensitivity caseSensitivity) { m_rightSortCaseSensitivity = caseSensitivity; }
    void setRightSortOrder(Qt::SortOrder order) { m_rightSortOrder = order; }

    QDir::Filters getLeftFilterSettings() { return m_leftFilterSettings; }
    void setLeftFilterSettings(QDir::Filters leftFilterSettings) { m_leftFilterSettings = leftFilterSettings; }

    QDir::Filters getRightFilterSettings() { return m_rightFilterSettings; }
    void setRightFilterSettings(QDir::Filters rightFilterSettings) { m_rightFilterSettings = rightFilterSettings; }

    DragAndDropBehaviorType getDragAndDropBehaviorType() { return m_dragAndDropBehaviorType; }
    void setDragAndDropBehaviorType(DragAndDropBehaviorType behaviorType) { m_dragAndDropBehaviorType = behaviorType; }

    bool getAutoDialogCloseCopy() { return m_autoDialogCloseCopy; }
    void setAutoDialogCloseCopy(bool autoClose) { m_autoDialogCloseCopy = autoClose; }

    bool getAutoDialogCloseMove() { return m_autoDialogCloseMove; }
    void setAutoDialogCloseMove(bool autoClose) { m_autoDialogCloseMove = autoClose; }

    bool getAutoDialogCloseRemove() { return m_autoDialogCloseRemove; }
    void setAutoDialogCloseRemove(bool autoClose) { m_autoDialogCloseRemove = autoClose; }

    bool getAllowCursorAround() { return m_allowCursorAround; }
    void setAllowCursorAround(bool allow) { m_allowCursorAround = allow; }

    bool getMoveCursorOpenViewer() { return m_moveCursorOpenViewer; }
    void setMoveCursorOpenViewer(bool enable) { m_moveCursorOpenViewer = enable; }

    bool getConfirmQuit() { return m_confirmQuit; }
    void setConfirmQuit(bool confirmQuit) { m_confirmQuit = confirmQuit; }

    FileSizeFormatType getSinglePaneFileSizeFormatType() { return m_singlePaneFileSizeFormatType; }
    void setSinglePaneFileSizeFormatType(FileSizeFormatType formatType) { m_singlePaneFileSizeFormatType = formatType; }

    bool getSinglePaneFileSizeDetailCommaEnable() { return m_singlePaneFileSizeDetailCommaEnable; }
    void setSinglePaneFileSizeDetailCommaEnable(bool enable) { m_singlePaneFileSizeDetailCommaEnable = enable; }

    DateFormatType getSinglePaneDateFormatType() { return m_singlePaneDateFormatType; }
    void setSinglePaneDateFormatType(DateFormatType formatType) { m_singlePaneDateFormatType = formatType; }

    QString getSinglePaneDateFormatOriginalString() { return m_singlePaneDateFormatOriginalString; }
    void setSinglePaneDateFormatOriginalString(const QString& str) { m_singlePaneDateFormatOriginalString = str; }

    FileSizeFormatType getDualPaneFileSizeFormatType() { return m_dualPaneFileSizeFormatType; }
    void setDualPaneFileSizeFormatType(FileSizeFormatType formatType) { m_dualPaneFileSizeFormatType = formatType; }

    bool getDualPaneFileSizeDetailCommaEnable() { return m_dualPaneFileSizeDetailCommaEnable; }
    void setDualPaneFileSizeDetailCommaEnable(bool enable) { m_dualPaneFileSizeDetailCommaEnable = enable; }

    DateFormatType getDualPaneDateFormatType() { return m_dualPaneDateFormatType; }
    void setDualPaneDateFormatType(DateFormatType formatType) { m_dualPaneDateFormatType = formatType; }

    QString getDualPaneDateFormatOriginalString() { return m_dualPaneDateFormatOriginalString; }
    void setDualPaneDateFormatOriginalString(const QString& str) { m_dualPaneDateFormatOriginalString = str; }

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

    QColor getValueColorSetting(const QString& key, const QColor& defColor);

    SectionType getValueSortSectionType(const QString& prefix);
    SortDirsType getValueSortDirsType(const QString& prefix);
    bool getValueSortDotFirst(const QString& prefix);
    Qt::CaseSensitivity getValueSortCaseSensitivity(const QString& prefix);
    Qt::SortOrder getValueSortOrder(const QString& prefix);
    void setValueSortSectionType(SectionType sectionType, const QString& prefix);
    void setValueSortDirsType(SortDirsType dirsType, const QString& prefix);
    void setValueSortDotFirst(bool dotFirst, const QString& prefix);
    void setValueSortCaseSensitivity(Qt::CaseSensitivity caseSensitivity, const QString& prefix);
    void setValueSortOrder(Qt::SortOrder order, const QString& prefix);

    QDir::Filters getValueFilterSettings(const QString& prefix);
    void setValueFilterSettings(QDir::Filters filterSettings, const QString& prefix);

    static Settings* s_instance;

    SizeAtStartup m_sizeAtStartupType = SizeAtStartup::Default;
    QSize m_sizeAtStartup = QSize(0, 0);

    PositionAtStartup m_positionAtStartupType = PositionAtStartup::Default;
    QPoint m_positionAtStartup = QPoint(0, 0);

    PaneMode m_paneMode = PaneMode::Default;
    PaneType m_activePane = PaneType::Default;

    FolderAtStartup m_leftFolderAtStartup = FolderAtStartup::Default;
    QString m_leftFolderPath = "";
    FolderAtStartup m_rightFolderAtStartup = FolderAtStartup::Default;
    QString m_rightFolderPath = "";

    SectionType m_leftSortSectionType = SectionType::FileName;
    SortDirsType m_leftSortDirsType = SortDirsType::NoSpecify;
    bool m_leftSortDotFirst = true;
    Qt::CaseSensitivity m_leftSortCaseSensitivity = Qt::CaseInsensitive;
    Qt::SortOrder m_leftSortOrder = Qt::AscendingOrder;
    SectionType m_rightSortSectionType = SectionType::FileName;
    SortDirsType m_rightSortDirsType = SortDirsType::NoSpecify;
    bool m_rightSortDotFirst = true;
    Qt::CaseSensitivity m_rightSortCaseSensitivity = Qt::CaseInsensitive;
    Qt::SortOrder m_rightSortOrder = Qt::AscendingOrder;

    QDir::Filters m_leftFilterSettings = DEFAULT_FILTER_FLAGS;
    QDir::Filters m_rightFilterSettings = DEFAULT_FILTER_FLAGS;

    DragAndDropBehaviorType m_dragAndDropBehaviorType = DragAndDropBehaviorType::Default;

    bool m_autoDialogCloseCopy = false;
    bool m_autoDialogCloseMove = false;
    bool m_autoDialogCloseRemove = false;

    bool m_allowCursorAround = false;

    bool m_moveCursorOpenViewer = false;

    bool m_confirmQuit = true;

    FileSizeFormatType m_singlePaneFileSizeFormatType = FileSizeFormatType::Default;
    bool m_singlePaneFileSizeDetailCommaEnable = false;

    FileSizeFormatType m_dualPaneFileSizeFormatType = FileSizeFormatType::Default;
    bool m_dualPaneFileSizeDetailCommaEnable = false;

    DateFormatType m_singlePaneDateFormatType = DateFormatType::Default;
    QString m_singlePaneDateFormatOriginalString = "yyyy-MM-dd HH:mm:ss";

    DateFormatType m_dualPaneDateFormatType = DateFormatType::Default;
    QString m_dualPaneDateFormatOriginalString = "yyyy-MM-dd HH:mm:ss";

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
        { "textViewer_selected_text",         "#000000", },
        { "textViewer_selected_background",   "#b2d7ff", },
        { "textViewer_lineNumber_text",       "#000000", },
        { "textViewer_lineNumber_background", "#dfdfdf", },

        { "hexViewer_text",                 "#000000", },
        { "hexViewer_background",           "#ffffff", },
        { "hexViewer_selected_text",        "#000000", },
        { "hexViewer_selected_background",  "#b2d7ff", },
        { "hexViewer_address_text",         "#000000", },
        { "hexViewer_address_background",   "#dfdfdf", },
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
        { "hexViewer",  getDefaultFixedFont(), },
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
