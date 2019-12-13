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
#include "default_settings.h"

namespace Farman
{

class Settings : public QSettings
{
    Q_OBJECT

public:
    static void create();
    static void destroy();

    static Settings* getInstance();

    void initialize();

    void flush();

    qint64 getCopyUnitSize() { return m_copyUnitSize; }
    void setCopyUnitSize(qint64 size) { m_copyUnitSize = size; }

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

    FolderAtStartup getFolderAtStartup(PaneType pane) { return m_folderAtStartup[static_cast<int>(pane)]; }
    void setFolderAtStartup(PaneType pane, FolderAtStartup folderAtStartup) { m_folderAtStartup[static_cast<int>(pane)] = folderAtStartup; }

    QString getFolderPath(PaneType pane) { return m_folderPath[static_cast<int>(pane)]; }
    void setFolderPath(PaneType pane, const QString& folderPath) { m_folderPath[static_cast<int>(pane)] = folderPath; }

    SectionType getSortSectionType(PaneType pane) { return m_sortSectionType[static_cast<int>(pane)]; }
    SectionType getSortSectionType2nd(PaneType pane) { return m_sortSectionType2nd[static_cast<int>(pane)]; }
    SortDirsType getSortDirsType(PaneType pane) { return m_sortDirsType[static_cast<int>(pane)]; }
    bool getSortDotFirst(PaneType pane) { return m_sortDotFirst[static_cast<int>(pane)]; }
    Qt::CaseSensitivity getSortCaseSensitivity(PaneType pane) { return m_sortCaseSensitivity[static_cast<int>(pane)]; }
    Qt::SortOrder getSortOrder(PaneType pane) { return m_sortOrder[static_cast<int>(pane)]; }
    void setSortSectionType(PaneType pane, SectionType sectionType) { m_sortSectionType[static_cast<int>(pane)] = sectionType; }
    void setSortSectionType2nd(PaneType pane, SectionType sectionType2nd) { m_sortSectionType2nd[static_cast<int>(pane)] = sectionType2nd; }
    void setSortDirsType(PaneType pane, SortDirsType dirsType) { m_sortDirsType[static_cast<int>(pane)] = dirsType; }
    void setSortDotFirst(PaneType pane, bool dotFirst) { m_sortDotFirst[static_cast<int>(pane)] = dotFirst; }
    void setSortCaseSensitivity(PaneType pane, Qt::CaseSensitivity caseSensitivity) { m_sortCaseSensitivity[static_cast<int>(pane)] = caseSensitivity; }
    void setSortOrder(PaneType pane, Qt::SortOrder order) { m_sortOrder[static_cast<int>(pane)] = order; }

    AttrFilterFlags getAttrFilterSettings(PaneType pane) const { return m_attrFilterSettings[static_cast<int>(pane)]; }
    void setAttrFilterSettings(PaneType pane, AttrFilterFlags attrFilterSettings) { m_attrFilterSettings[static_cast<int>(pane)] = attrFilterSettings; }

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

    bool getFolderColorTopPriority() { return m_folderColorTopPriority; }
    void setFolderColorTopPriority(bool enabled) { m_folderColorTopPriority = enabled; }

    qreal getFolderViewRowHeight() { return m_folderViewRowHeight; }
    void setFolderViewRowHeight(qreal height) { m_folderViewRowHeight = height; }

    bool getEnableInactiveFontColor() { return m_enableInactiveFontColor; }
    void setEnableInactiveFontColor(bool enabled) { m_enableInactiveFontColor = enabled; }

    int getCursorWidth() { return m_cursorWidth; }
    void setCursorWidth(int cursorWidth) { m_cursorWidth = cursorWidth; }

    bool getConsoleVisible() { return m_consoleVisible; }
    void setConsoleVisible(bool visible) { m_consoleVisible = visible; }

    const QList<QString>& getHistoryList(PaneType pane);
    void setHistoryList(PaneType pane, const QList<QString>& historyList);

    bool getTextViewerShowLineNumber() { return m_textViewerShowLineNumber; }
    void setTextViewerShowLineNumber(bool show) { m_textViewerShowLineNumber = show; }

    bool getTextViewerWordWrap() { return m_textViewerWordWrap; }
    void setTextViewerWordWrap(bool enabled) { m_textViewerWordWrap = enabled; }

    const QList<QString>& getTextViewerEncodeList() { return m_textViewerEncodeList; }
    void setTextViewerEncodeList(const QList<QString>& encodeList) { m_textViewerEncodeList = encodeList; }

    bool getImageViewerFitInView() { return m_imageViewerFitInView; }
    void setImageViewerFitInView(bool fitInView) { m_imageViewerFitInView = fitInView; }

    ImageViewerTransparentBGType getImageViewerTransparentBGType() { return m_imageViewerTransparentBGType; }
    void setImageViewerTransparentBGType(ImageViewerTransparentBGType bgType) { m_imageViewerTransparentBGType = bgType; }

    QString getTextEditorPath() { return m_textEditorPath; }
    void setTextEditorPath(const QString& textEditorPath) { m_textEditorPath = textEditorPath; }
    QString getTextEditorArgs() { return m_textEditorArgs; }
    void setTextEditorArgs(const QString& textEditorArgs) { m_textEditorArgs = textEditorArgs; }

private:
    Settings();

    QColor getValueColorSetting(const QString& key, const QColor& defColor);

    SectionType getValueSortSectionType(const QString& prefix);
    SectionType getValueSortSectionType2nd(const QString& prefix);
    SortDirsType getValueSortDirsType(const QString& prefix);
    bool getValueSortDotFirst(const QString& prefix);
    Qt::CaseSensitivity getValueSortCaseSensitivity(const QString& prefix);
    Qt::SortOrder getValueSortOrder(const QString& prefix);
    void setValueSortSectionType(SectionType sectionType, const QString& prefix);
    void setValueSortSectionType2nd(SectionType sectionType2nd, const QString& prefix);
    void setValueSortDirsType(SortDirsType dirsType, const QString& prefix);
    void setValueSortDotFirst(bool dotFirst, const QString& prefix);
    void setValueSortCaseSensitivity(Qt::CaseSensitivity caseSensitivity, const QString& prefix);
    void setValueSortOrder(Qt::SortOrder order, const QString& prefix);

    AttrFilterFlags getValueAttrFilterSettings(const QString& prefix) const;
    void setValueAttrFilterSettings(AttrFilterFlags attrFilterSettings, const QString& prefix);

    void getValueHistoryList(QList<QString>& historyList, const QString& prefix);
    void setValueHistoryList(const QList<QString>& historyList, const QString& prefix);

    static Settings* s_instance;

    qint64 m_copyUnitSize = DEFAULT_COPY_UNIT_SIZE;

    SizeAtStartup m_sizeAtStartupType = DEFAULT_SIZE_AT_STARTUP_TYPE;
    QSize m_sizeAtStartup = DEFAULT_SIZE_AT_STARTUP;

    PositionAtStartup m_positionAtStartupType = DEFAULT_POSITION_AT_STARTUP_TYPE;
    QPoint m_positionAtStartup = DEFAULT_POSITION_AT_STARTUP;

    PaneMode m_paneMode = DEFAULT_PANE_MODE;
    PaneType m_activePane = DEFAULT_ACTIVE_PANE;

    FolderAtStartup m_folderAtStartup[static_cast<int>(PaneType::PaneTypeNum)] = { DEFAULT_FOLDER_AT_STARTUP_TYPE, DEFAULT_FOLDER_AT_STARTUP_TYPE };
    QString         m_folderPath[static_cast<int>(PaneType::PaneTypeNum)]      = { DEFAULT_FOLDER_PATH_AT_STARTUP, DEFAULT_FOLDER_PATH_AT_STARTUP };

    SectionType         m_sortSectionType[static_cast<int>(PaneType::PaneTypeNum)]     = { DEFAULT_SORT_SECTION_TYPE,     DEFAULT_SORT_SECTION_TYPE     };
    SectionType         m_sortSectionType2nd[static_cast<int>(PaneType::PaneTypeNum)]  = { DEFAULT_SORT_SECTION_TYPE_2ND, DEFAULT_SORT_SECTION_TYPE_2ND };
    SortDirsType        m_sortDirsType[static_cast<int>(PaneType::PaneTypeNum)]        = { DEFAULT_SORT_DIRS_TYPE,        DEFAULT_SORT_DIRS_TYPE        };
    bool                m_sortDotFirst[static_cast<int>(PaneType::PaneTypeNum)]        = { DEFAULT_SORT_DOT_FIRST,        DEFAULT_SORT_DOT_FIRST        };
    Qt::CaseSensitivity m_sortCaseSensitivity[static_cast<int>(PaneType::PaneTypeNum)] = { DEFAULT_SORT_CASE_SENSITIVITY, DEFAULT_SORT_CASE_SENSITIVITY };
    Qt::SortOrder       m_sortOrder[static_cast<int>(PaneType::PaneTypeNum)]           = { DEFAULT_SORT_ORDER,            DEFAULT_SORT_ORDER            };

    AttrFilterFlags m_attrFilterSettings[static_cast<int>(PaneType::PaneTypeNum)] = { DEFAULT_ATTR_FILTER_FLAGS, DEFAULT_ATTR_FILTER_FLAGS };

    DragAndDropBehaviorType m_dragAndDropBehaviorType = DEFAULT_DRAG_AND_DROP_BEHAVIOR_TYPE;

    bool m_autoDialogCloseCopy = DEFAULT_AUTO_DIALOG_CLOSE_COPY;
    bool m_autoDialogCloseMove = DEFAULT_AUTO_DIALOG_CLOSE_MOVE;
    bool m_autoDialogCloseRemove = DEFAULT_AUTO_DIALOG_CLOSE_REMOVE;

    bool m_allowCursorAround = DEFAULT_ALLOW_CURSOR_AROUND;

    bool m_moveCursorOpenViewer = DEFAULT_MOVE_CURSOR_OPEN_VIEWER;

    bool m_confirmQuit = DEFAULT_CONFIRM_QUIT;

    FileSizeFormatType m_singlePaneFileSizeFormatType = DEFAULT_SINGLE_PANE_FILE_SIZE_FORMAT_TYPE;
    bool m_singlePaneFileSizeDetailCommaEnable = DEFAULT_SINGLE_PANE_FILE_SIZE_DETAIL_COMMA_ENABLE;

    FileSizeFormatType m_dualPaneFileSizeFormatType = DEFAULT_DUAL_PANE_FILE_SIZE_FORMAT_TYPE;
    bool m_dualPaneFileSizeDetailCommaEnable = DEFAULT_DUAL_PANE_FILE_SIZE_DETAIL_COMMA_ENABLE;

    DateFormatType m_singlePaneDateFormatType = DEFAULT_SINGLE_PANE_DATE_FORMAT_TYPE;
    QString m_singlePaneDateFormatOriginalString = DEFAULT_SINGLE_PANE_DATE_FORMAT_ORIGINAL_STRING;

    DateFormatType m_dualPaneDateFormatType = DEFAULT_DUAL_PANE_DATE_FORMAT_TYPE;
    QString m_dualPaneDateFormatOriginalString = DEFAULT_DUAL_PANE_DATE_FORMAT_ORIGINAL_STRING;

    QMap<QString, QColor> m_colorSettings = DEFAULT_COLOR_SETTINGS;

    QMap<QString, QFont> m_fontSettings;

    bool m_folderColorTopPriority = DEFAULT_FOLDER_COLOR_TOP_PRIORITY;

    qreal m_folderViewRowHeight = DEFAULT_FOLDER_VIEW_ROW_HEIGHT;

    bool m_enableInactiveFontColor = DEFAULT_ENABLE_INACTIVE_FONT_COLOR;

    int m_cursorWidth = DEFAULT_CURSOR_WIDTH;

    bool m_consoleVisible = DEFAULT_CONSOLE_VISIBLE;

    QList<QString> m_historyList[static_cast<int>(PaneType::PaneTypeNum)];

    bool m_textViewerShowLineNumber = DEFAULT_TEXT_VIEWER_SHOW_LINE_NUMBER;
    bool m_textViewerWordWrap = DEFAULT_TEXT_VIEWER_WORD_WRAP;

    QList<QString> m_textViewerEncodeList;

    bool m_imageViewerFitInView = DEFAULT_IMAGE_VIEWER_FIT_IN_VIEW;
    ImageViewerTransparentBGType m_imageViewerTransparentBGType = DEFAULT_IMAGE_VIEWER_TRANSPARENT_BG_TYPE;

    QString m_textEditorPath = DEFAULT_TEXT_EDITOR_PATH;
    QString m_textEditorArgs = DEFAULT_TEXT_EDITOR_ARGS;
};

}

#endif // SETTINGS_H
