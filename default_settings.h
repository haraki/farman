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

static const SizeAtStartup DEFAULT_SIZE_AT_STARTUP_TYPE = SizeAtStartup::Default;
static const QSize DEFAULT_SIZE_AT_STARTUP = QSize(0, 0);

static const PositionAtStartup DEFAULT_POSITION_AT_STARTUP_TYPE = PositionAtStartup::Default;
static const QPoint DEFAULT_POSITION_AT_STARTUP = QPoint(0, 0);

static const PaneMode DEFAULT_PANE_MODE = PaneMode::Dual;
static const PaneType DEFAULT_ACTIVE_PANE = PaneType::Left;

static const FolderAtStartup DEFAULT_FOLDER_AT_STARTUP_TYPE = FolderAtStartup::Default;
static const QString DEFAULT_FOLDER_PATH_AT_STARTUP = "";

static const SectionType DEFAULT_SORT_SECTION_TYPE = SectionType::FileName;
static const SortDirsType DEFAULT_SORT_DIRS_TYPE = SortDirsType::NoSpecify;
static const bool DEFAULT_SORT_DOT_FIRST = true;
static const Qt::CaseSensitivity DEFAULT_SORT_CASE_SENSITIVITY = Qt::CaseInsensitive;
static const Qt::SortOrder DEFAULT_SORT_ORDER = Qt::AscendingOrder;

static const QDir::Filters DEFAULT_FILTER_FLAGS = FIX_FILTER_FLAGS;

static const OverwriteMethodType DEFAULT_OVERWRITE_METHOD_TYPE = OverwriteMethodType::Overwrite;

static const DragAndDropBehaviorType DEFAULT_DRAG_AND_DROP_BEHAVIOR_TYPE = DragAndDropBehaviorType::Select;

static const bool DEFAULT_AUTO_DIALOG_CLOSE_COPY = false;
static const bool DEFAULT_AUTO_DIALOG_CLOSE_MOVE = false;
static const bool DEFAULT_AUTO_DIALOG_CLOSE_REMOVE = false;

static const bool DEFAULT_ALLOW_CURSOR_AROUND = false;

static const bool DEFAULT_MOVE_CURSOR_OPEN_VIEWER = false;

static const bool DEFAULT_CONFIRM_QUIT = true;

static const FileSizeFormatType DEFAULT_SINGLE_PANE_FILE_SIZE_FORMAT_TYPE = FileSizeFormatType::SI;
static const bool DEFAULT_SINGLE_PANE_FILE_SIZE_DETAIL_COMMA_ENABLE = false;

static const FileSizeFormatType DEFAULT_DUAL_PANE_FILE_SIZE_FORMAT_TYPE = FileSizeFormatType::SI;
static const bool DEFAULT_DUAL_PANE_FILE_SIZE_DETAIL_COMMA_ENABLE = false;

static const DateFormatType DEFAULT_SINGLE_PANE_DATE_FORMAT_TYPE = DateFormatType::Default;
static const QString DEFAULT_SINGLE_PANE_DATE_FORMAT_ORIGINAL_STRING = "yyyy-MM-dd HH:mm:ss";

static const DateFormatType DEFAULT_DUAL_PANE_DATE_FORMAT_TYPE = DateFormatType::Original;
static const QString DEFAULT_DUAL_PANE_DATE_FORMAT_ORIGINAL_STRING = "yyyy-MM-dd HH:mm";

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

static const QMap<QString, QFont> DEFAULT_FONT_SETTINGS =
{
    { "folderView", getDefaultFixedFont(), },
    { "folderPath", getDefaultFixedFont(), },
    { "console",    getDefaultFixedFont(), },
    { "textViewer", getDefaultFixedFont(), },
    { "hexViewer",  getDefaultFixedFont(), },
};

static const int DEFAULT_CURSOR_WIDTH = 1;

static const bool DEFAULT_CONSOLE_VISIBLE = true;

static const bool DEFAULT_IMAGE_VIEWER_FIT_IN_VIEW = true;
static const ImageViewerBGType DEFAULT_IMAGE_VIEWER_BG_TYPE = ImageViewerBGType::Checkered;

static const bool DEFAULT_TEXT_VIEWER_SHOW_LINE_NUMBER = true;
static const bool DEFAULT_TEXT_VIEWER_WORD_WRAP = false;

static const QString DEFAULT_TEXT_EDITOR_PATH = "";
static const QString DEFAULT_TEXT_EDITOR_ARGS = "";

}           // namespace Farman

#endif // DEFAULT_SETTINGS_H
