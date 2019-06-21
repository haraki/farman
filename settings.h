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

    FolderAtStartup getLeftFolderAtStartup() { return m_leftFolderAtStartup; }
    void setLeftFolderAtStartup(FolderAtStartup leftFolderAtStartup) { m_leftFolderAtStartup = leftFolderAtStartup; }

    QString getLeftFolderPath() { return m_leftFolderPath; }
    void setLeftFolderPath(const QString& leftFolderPath) { m_leftFolderPath = leftFolderPath; }

    FolderAtStartup getRightFolderAtStartup() { return m_rightFolderAtStartup; }
    void setRightFolderAtStartup(FolderAtStartup rightFolderAtStartup) { m_rightFolderAtStartup = rightFolderAtStartup; }

    QString getRightFolderPath() { return m_rightFolderPath; }
    void setRightFolderPath(const QString& rightFolderPath) { m_rightFolderPath = rightFolderPath; }

    SectionType getLeftSortSectionType() { return m_leftSortSectionType; }
    SectionType getLeftSortSectionType2nd() { return m_leftSortSectionType2nd; }
    SortDirsType getLeftSortDirsType() { return m_leftSortDirsType; }
    bool getLeftSortDotFirst() { return m_leftSortDotFirst; }
    Qt::CaseSensitivity getLeftSortCaseSensitivity() { return m_leftSortCaseSensitivity; }
    Qt::SortOrder getLeftSortOrder() { return m_leftSortOrder; }
    void setLeftSortSectionType(SectionType sectionType) { m_leftSortSectionType = sectionType; }
    void setLeftSortSectionType2nd(SectionType sectionType2nd) { m_leftSortSectionType2nd = sectionType2nd; }
    void setLeftSortDirsType(SortDirsType dirsType) { m_leftSortDirsType = dirsType; }
    void setLeftSortDotFirst(bool dotFirst) { m_leftSortDotFirst = dotFirst; }
    void setLeftSortCaseSensitivity(Qt::CaseSensitivity caseSensitivity) { m_leftSortCaseSensitivity = caseSensitivity; }
    void setLeftSortOrder(Qt::SortOrder order) { m_leftSortOrder = order; }

    SectionType getRightSortSectionType() { return m_rightSortSectionType; }
    SectionType getRightSortSectionType2nd() { return m_rightSortSectionType2nd; }
    SortDirsType getRightSortDirsType() { return m_rightSortDirsType; }
    bool getRightSortDotFirst() { return m_rightSortDotFirst; }
    Qt::CaseSensitivity getRightSortCaseSensitivity() { return m_rightSortCaseSensitivity; }
    Qt::SortOrder getRightSortOrder() { return m_rightSortOrder; }
    void setRightSortSectionType(SectionType sectionType) { m_rightSortSectionType = sectionType; }
    void setRightSortSectionType2nd(SectionType sectionType2nd) { m_rightSortSectionType2nd = sectionType2nd; }
    void setRightSortDirsType(SortDirsType dirsType) { m_rightSortDirsType = dirsType; }
    void setRightSortDotFirst(bool dotFirst) { m_rightSortDotFirst = dotFirst; }
    void setRightSortCaseSensitivity(Qt::CaseSensitivity caseSensitivity) { m_rightSortCaseSensitivity = caseSensitivity; }
    void setRightSortOrder(Qt::SortOrder order) { m_rightSortOrder = order; }

    FilterFlags getLeftFilterSettings() const { return m_leftFilterSettings; }
    void setLeftFilterSettings(FilterFlags filterSettings) { m_leftFilterSettings = filterSettings; }
    QStringList getLeftNameMaskFilterSettings() const { return m_leftNameMaskFilterSettings; }
    void setLeftNameMaskFilterSettings(const QStringList& nameMaskFilterSettings) { m_leftNameMaskFilterSettings = nameMaskFilterSettings; }

    FilterFlags getRightFilterSettings() const { return m_rightFilterSettings; }
    void setRightFilterSettings(FilterFlags filterSettings) { m_rightFilterSettings = filterSettings; }
    QStringList getRightNameMaskFilterSettings() const { return m_rightNameMaskFilterSettings; }
    void setRightNameMaskFilterSettings(const QStringList& nameMaskFilterSettings) { m_rightNameMaskFilterSettings = nameMaskFilterSettings; }

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

    bool getConsoleVisible() { return m_consoleVisible; }
    void setConsoleVisible(bool visible) { m_consoleVisible = visible; }

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

    const QList<QPair<QString, QString>>& getBookmarkDirPathList() { return m_bookmarkDirPathList; }
    void setBookmarkDirPathList(const QList<QPair<QString, QString>>& dirPathList) { m_bookmarkDirPathList = dirPathList; }
    QPair<QString, QString> getBookmarkDirPath(const QString& name);
    QPair<QString, QString> getBookmarkDirPath(int index);
    int searchBookmarkDirPath(const QString& path);
    int insertBookmarkDirPath(const QString dirPath, int index = -1);
    int insertBookmarkDirPath(const QPair<QString, QString>& dirPath, int index = -1);
    int removeBookmarkDirPath(const QPair<QString, QString>& dirPath);
    int removeBookmarkDirPath(int index);

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

    FilterFlags getValueFilterSettings(const QString& prefix) const;
    void setValueFilterSettings(FilterFlags filterSettings, const QString& prefix);
    QStringList getValueNameMaskFilterSettings(const QString& prefix) const;
    void setValueNameMaskFilterSettings(const QStringList& nameMaskFilterSettings, const QString& prefix);

    static Settings* s_instance;

    qint64 m_copyUnitSize = DEFAULT_COPY_UNIT_SIZE;

    SizeAtStartup m_sizeAtStartupType = DEFAULT_SIZE_AT_STARTUP_TYPE;
    QSize m_sizeAtStartup = DEFAULT_SIZE_AT_STARTUP;

    PositionAtStartup m_positionAtStartupType = DEFAULT_POSITION_AT_STARTUP_TYPE;
    QPoint m_positionAtStartup = DEFAULT_POSITION_AT_STARTUP;

    PaneMode m_paneMode = DEFAULT_PANE_MODE;
    PaneType m_activePane = DEFAULT_ACTIVE_PANE;

    FolderAtStartup m_leftFolderAtStartup = DEFAULT_FOLDER_AT_STARTUP_TYPE;
    QString m_leftFolderPath = DEFAULT_FOLDER_PATH_AT_STARTUP;
    FolderAtStartup m_rightFolderAtStartup = DEFAULT_FOLDER_AT_STARTUP_TYPE;
    QString m_rightFolderPath = DEFAULT_FOLDER_PATH_AT_STARTUP;

    SectionType m_leftSortSectionType = DEFAULT_SORT_SECTION_TYPE;
    SectionType m_leftSortSectionType2nd = DEFAULT_SORT_SECTION_TYPE_2ND;
    SortDirsType m_leftSortDirsType = DEFAULT_SORT_DIRS_TYPE;
    bool m_leftSortDotFirst = DEFAULT_SORT_DOT_FIRST;
    Qt::CaseSensitivity m_leftSortCaseSensitivity = DEFAULT_SORT_CASE_SENSITIVITY;
    Qt::SortOrder m_leftSortOrder = DEFAULT_SORT_ORDER;
    SectionType m_rightSortSectionType = DEFAULT_SORT_SECTION_TYPE;
    SectionType m_rightSortSectionType2nd = DEFAULT_SORT_SECTION_TYPE_2ND;
    SortDirsType m_rightSortDirsType = DEFAULT_SORT_DIRS_TYPE;
    bool m_rightSortDotFirst = DEFAULT_SORT_DOT_FIRST;
    Qt::CaseSensitivity m_rightSortCaseSensitivity = DEFAULT_SORT_CASE_SENSITIVITY;
    Qt::SortOrder m_rightSortOrder = DEFAULT_SORT_ORDER;

    FilterFlags m_leftFilterSettings = DEFAULT_FILTER_FLAGS;
    QStringList m_leftNameMaskFilterSettings = QString(DEFAULT_NAME_MASK_FILTERS).simplified().split(' ', QString::SkipEmptyParts);
    FilterFlags m_rightFilterSettings = DEFAULT_FILTER_FLAGS;
    QStringList m_rightNameMaskFilterSettings = QString(DEFAULT_NAME_MASK_FILTERS).simplified().split(' ', QString::SkipEmptyParts);

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

    int m_cursorWidth = DEFAULT_CURSOR_WIDTH;

    bool m_consoleVisible = DEFAULT_CONSOLE_VISIBLE;

    bool m_textViewerShowLineNumber = DEFAULT_TEXT_VIEWER_SHOW_LINE_NUMBER;
    bool m_textViewerWordWrap = DEFAULT_TEXT_VIEWER_WORD_WRAP;

    QList<QString> m_textViewerEncodeList;

    bool m_imageViewerFitInView = DEFAULT_IMAGE_VIEWER_FIT_IN_VIEW;
    ImageViewerTransparentBGType m_imageViewerTransparentBGType = DEFAULT_IMAGE_VIEWER_TRANSPARENT_BG_TYPE;

    QString m_textEditorPath = DEFAULT_TEXT_EDITOR_PATH;
    QString m_textEditorArgs = DEFAULT_TEXT_EDITOR_ARGS;

    QList<QPair<QString, QString>> m_bookmarkDirPathList;
};

}

#endif // SETTINGS_H
