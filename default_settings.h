#ifndef DEFAULT_SETTINGS_H
#define DEFAULT_SETTINGS_H

#include <qnamespace.h>
#include <QMap>
#include <QString>
#include <QColor>
#include <QSize>
#include <QPoint>
#include <QFont>
#include "types.h"

namespace Farman
{

static Q_DECL_CONSTEXPR qint32 DEFAULT_COPY_UNIT_SIZE = 1 * 1024 * 1024;

static Q_DECL_CONSTEXPR SizeAtStartup DEFAULT_SIZE_AT_STARTUP_TYPE = SizeAtStartup::Default;
static Q_DECL_CONSTEXPR QSize DEFAULT_SIZE_AT_STARTUP = QSize(0, 0);

static Q_DECL_CONSTEXPR PositionAtStartup DEFAULT_POSITION_AT_STARTUP_TYPE = PositionAtStartup::Default;
static Q_DECL_CONSTEXPR QPoint DEFAULT_POSITION_AT_STARTUP = QPoint(0, 0);

static Q_DECL_CONSTEXPR PaneMode DEFAULT_PANE_MODE = PaneMode::Dual;
static Q_DECL_CONSTEXPR PaneType DEFAULT_ACTIVE_PANE = PaneType::Left;

static Q_DECL_CONSTEXPR FolderAtStartup DEFAULT_FOLDER_AT_STARTUP_TYPE = FolderAtStartup::Default;
static Q_DECL_CONSTEXPR char DEFAULT_FOLDER_PATH_AT_STARTUP[] = "";

static Q_DECL_CONSTEXPR SectionType DEFAULT_SORT_SECTION_TYPE = SectionType::FileName;
static Q_DECL_CONSTEXPR SortDirsType DEFAULT_SORT_DIRS_TYPE = SortDirsType::NoSpecify;
static Q_DECL_CONSTEXPR bool DEFAULT_SORT_DOT_FIRST = true;
static Q_DECL_CONSTEXPR Qt::CaseSensitivity DEFAULT_SORT_CASE_SENSITIVITY = Qt::CaseInsensitive;
static Q_DECL_CONSTEXPR Qt::SortOrder DEFAULT_SORT_ORDER = Qt::AscendingOrder;

static Q_DECL_CONSTEXPR FilterFlags DEFAULT_FILTER_FLAGS = FilterFlag::Hidden | FilterFlag::System;

static Q_DECL_CONSTEXPR char DEFAULT_NAME_MASK_FILTERS[] = "*";

static Q_DECL_CONSTEXPR OverwriteMethodType DEFAULT_OVERWRITE_METHOD_TYPE = OverwriteMethodType::Overwrite;

static Q_DECL_CONSTEXPR DragAndDropBehaviorType DEFAULT_DRAG_AND_DROP_BEHAVIOR_TYPE = DragAndDropBehaviorType::Select;

static Q_DECL_CONSTEXPR bool DEFAULT_AUTO_DIALOG_CLOSE_COPY = false;
static Q_DECL_CONSTEXPR bool DEFAULT_AUTO_DIALOG_CLOSE_MOVE = false;
static Q_DECL_CONSTEXPR bool DEFAULT_AUTO_DIALOG_CLOSE_REMOVE = false;

static Q_DECL_CONSTEXPR bool DEFAULT_ALLOW_CURSOR_AROUND = false;

static Q_DECL_CONSTEXPR bool DEFAULT_MOVE_CURSOR_OPEN_VIEWER = false;

static Q_DECL_CONSTEXPR bool DEFAULT_CONFIRM_QUIT = true;

static Q_DECL_CONSTEXPR FileSizeFormatType DEFAULT_SINGLE_PANE_FILE_SIZE_FORMAT_TYPE = FileSizeFormatType::SI;
static Q_DECL_CONSTEXPR bool DEFAULT_SINGLE_PANE_FILE_SIZE_DETAIL_COMMA_ENABLE = false;

static Q_DECL_CONSTEXPR FileSizeFormatType DEFAULT_DUAL_PANE_FILE_SIZE_FORMAT_TYPE = FileSizeFormatType::SI;
static Q_DECL_CONSTEXPR bool DEFAULT_DUAL_PANE_FILE_SIZE_DETAIL_COMMA_ENABLE = false;

static Q_DECL_CONSTEXPR DateFormatType DEFAULT_SINGLE_PANE_DATE_FORMAT_TYPE = DateFormatType::Default;
static Q_DECL_CONSTEXPR char DEFAULT_SINGLE_PANE_DATE_FORMAT_ORIGINAL_STRING[] = "yyyy-MM-dd HH:mm:ss";

static Q_DECL_CONSTEXPR DateFormatType DEFAULT_DUAL_PANE_DATE_FORMAT_TYPE = DateFormatType::Original;
static Q_DECL_CONSTEXPR char DEFAULT_DUAL_PANE_DATE_FORMAT_ORIGINAL_STRING[] = "yyyy-MM-dd HH:mm";

static const QMap<QString, QColor> DEFAULT_COLOR_SETTINGS =
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

    { "imageViewer_transparent_background", "#808080", },

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

class FontSettingParams
{
public:
    FontSettingParams(const QString& family, int pointSize = -1, int weight = -1, bool italic = false) :
        m_family(family),
        m_pointSize(pointSize),
        m_weight(weight),
        m_italic(italic)
    {}
    FontSettingParams() = default;
    FontSettingParams(const FontSettingParams& params) = default;
    ~FontSettingParams() = default;

    bool isValid() const { return m_family.length() > 0; }

    const QString& getFamily() const { return m_family; }
    int getPointSize() const { return m_pointSize; }
    int getWeight() const { return m_weight; }
    bool getItalic() const { return m_italic; }

private:
    QString m_family = "";
    int m_pointSize = -1;
    int m_weight = -1;
    bool m_italic = false;
    qint64 :56;
};

#if defined(Q_OS_WIN)
#   define FONT_SETTING_PARAMS    FontSettingParams("Terminal", 14)
#elif defined(Q_OS_MAC)
#   define FONT_SETTING_PARAMS    FontSettingParams("Monaco", 13)
#else
#   define FONT_SETTING_PARAMS    FontSettingParams()
#endif

static const QMap<QString, FontSettingParams> DEFAULT_FONT_SETTING_PARAMS =
{
    { "folderView", FONT_SETTING_PARAMS, },
    { "folderPath", FONT_SETTING_PARAMS, },
    { "console",    FONT_SETTING_PARAMS, },
    { "textViewer", FONT_SETTING_PARAMS, },
    { "hexViewer",  FONT_SETTING_PARAMS, },
};

static Q_DECL_CONSTEXPR int DEFAULT_CURSOR_WIDTH = 1;

static Q_DECL_CONSTEXPR bool DEFAULT_CONSOLE_VISIBLE = true;

static Q_DECL_CONSTEXPR bool DEFAULT_TEXT_VIEWER_SHOW_LINE_NUMBER = true;
static Q_DECL_CONSTEXPR bool DEFAULT_TEXT_VIEWER_WORD_WRAP = false;

static Q_DECL_CONSTEXPR bool DEFAULT_IMAGE_VIEWER_FIT_IN_VIEW = true;
static Q_DECL_CONSTEXPR ImageViewerTransparentBGType DEFAULT_IMAGE_VIEWER_TRANSPARENT_BG_TYPE = ImageViewerTransparentBGType::Checkered;

#if defined(Q_OS_WIN)
static Q_DECL_CONSTEXPR char DEFAULT_TEXT_EDITOR_PATH[] = "C:\\Windows\\notepad.exe";
static Q_DECL_CONSTEXPR char DEFAULT_TEXT_EDITOR_ARGS[] = "$F";
#elif defined(Q_OS_MAC)
static Q_DECL_CONSTEXPR char DEFAULT_TEXT_EDITOR_PATH[] = "/Applications/TextEdit.app";
static Q_DECL_CONSTEXPR char DEFAULT_TEXT_EDITOR_ARGS[] = "$M";
#else
static Q_DECL_CONSTEXPR char DEFAULT_TEXT_EDITOR_PATH[] = "";
static Q_DECL_CONSTEXPR char DEFAULT_TEXT_EDITOR_ARGS[] = "$F";
#endif

}           // namespace Farman

#endif // DEFAULT_SETTINGS_H
