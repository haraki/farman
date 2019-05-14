#include <QTextCodec>
#include <QCoreApplication>
#include <QDebug>
#include "settings.h"

namespace Farman
{

Settings* Settings::s_instance = Q_NULLPTR;

void Settings::create()
{
    s_instance = new Settings();

    s_instance->initialize();
}

void Settings::destroy()
{
    delete s_instance;

    s_instance = Q_NULLPTR;
}

Settings* Settings::getInstance()
{
    return s_instance;
}

Settings::Settings()
    : QSettings(QSettings::IniFormat, QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName())
{
    this->setIniCodec("UTF-8");
}

void Settings::initialize()
{
    // Copy unit size
    m_copyUnitSize = value("main/copyUnitSize", DEFAULT_COPY_UNIT_SIZE).toLongLong();

    // Size at startup
    QString sizeAtStartupTypeValue = value("main/sizeAtStartupType").toString();
    m_sizeAtStartupType = (sizeAtStartupTypeValue == "default")  ? SizeAtStartup::Default :
                          (sizeAtStartupTypeValue == "lastTime") ? SizeAtStartup::LastTime :
                          (sizeAtStartupTypeValue == "fixed")    ? SizeAtStartup::Fixed :
                                                                   DEFAULT_SIZE_AT_STARTUP_TYPE;
    m_sizeAtStartup = value("main/sizeAtStartup", DEFAULT_SIZE_AT_STARTUP).toSize();

    // Position at startup
    QString positionAtStartupTypeValue = value("main/positionAtStartupType").toString();
    m_positionAtStartupType = (positionAtStartupTypeValue == "default")  ? PositionAtStartup::Default :
                              (positionAtStartupTypeValue == "lastTime") ? PositionAtStartup::LastTime :
                              (positionAtStartupTypeValue == "fixed")    ? PositionAtStartup::Fixed :
                                                                           DEFAULT_POSITION_AT_STARTUP_TYPE;
    m_positionAtStartup = value("main/positionAtStartup", DEFAULT_POSITION_AT_STARTUP).toPoint();

    // Pane
    QString paneModeValue = value("main/paneMode").toString();
    m_paneMode = (paneModeValue == "single") ? PaneMode::Single :
                 (paneModeValue == "dual")   ? PaneMode::Dual :
                                               DEFAULT_PANE_MODE;

    QString activePaneValue = value("main/activePane").toString();
    m_activePane = (activePaneValue == "left")  ? PaneType::Left :
                   (activePaneValue == "right") ? PaneType::Right :
                                                  DEFAULT_ACTIVE_PANE;

    // Folder at startup(Left)
    QString leftFolderAtStartupValue = value("main/leftFolderAtStartup").toString();
    m_leftFolderAtStartup = (leftFolderAtStartupValue == "default")  ? FolderAtStartup::Default :
                            (leftFolderAtStartupValue == "lastTime") ? FolderAtStartup::LastTime :
                            (leftFolderAtStartupValue == "fixed")    ? FolderAtStartup::Fixed :
                                                                       DEFAULT_FOLDER_AT_STARTUP_TYPE;
    m_leftFolderPath = value("main/leftFolderPath", DEFAULT_FOLDER_PATH_AT_STARTUP).toString();

    // Folder at startup(Right)
    QString rightFolderAtStartupValue = value("main/rightFolderAtStartup").toString();
    m_rightFolderAtStartup = (rightFolderAtStartupValue == "default")  ? FolderAtStartup::Default :
                             (rightFolderAtStartupValue == "lastTime") ? FolderAtStartup::LastTime :
                             (rightFolderAtStartupValue == "fixed")    ? FolderAtStartup::Fixed :
                                                                         DEFAULT_FOLDER_AT_STARTUP_TYPE;
    m_rightFolderPath = value("main/rightFolderPath", DEFAULT_FOLDER_PATH_AT_STARTUP).toString();

    // Left side Sort settings
    m_leftSortSectionType = getValueSortSectionType("left");
    m_leftSortDirsType = getValueSortDirsType("left");
    m_leftSortDotFirst = getValueSortDotFirst("left");
    m_leftSortCaseSensitivity = getValueSortCaseSensitivity("left");
    m_leftSortOrder = getValueSortOrder("left");

    // Right side Sort settings
    m_rightSortSectionType = getValueSortSectionType("right");
    m_rightSortDirsType = getValueSortDirsType("right");
    m_rightSortDotFirst = getValueSortDotFirst("right");
    m_rightSortCaseSensitivity = getValueSortCaseSensitivity("right");
    m_rightSortOrder = getValueSortOrder("right");

    // Left side Filter settings
    m_leftFilterSettings = getValueFilterSettings("left");

    // Right side Filter settings
    m_rightFilterSettings = getValueFilterSettings("right");

    // Drag & Drop behavior in FolderView
    QString behaviorTypeValue = value("main/dragAndDropBehaviorType").toString();
    m_dragAndDropBehaviorType = (behaviorTypeValue == "select") ? DragAndDropBehaviorType::Select :
                                (behaviorTypeValue == "copy")   ? DragAndDropBehaviorType::Copy :
                                (behaviorTypeValue == "move")   ? DragAndDropBehaviorType::Move :
                                                                  DEFAULT_DRAG_AND_DROP_BEHAVIOR_TYPE;

    // Auto dialog closing at successful operation.
    m_autoDialogCloseCopy = value("main/autoDialogCloseCopy", DEFAULT_AUTO_DIALOG_CLOSE_COPY).toBool();
    m_autoDialogCloseMove = value("main/autoDialogCloseMove", DEFAULT_AUTO_DIALOG_CLOSE_MOVE).toBool();
    m_autoDialogCloseRemove = value("main/autoDialogCloseRemove", DEFAULT_AUTO_DIALOG_CLOSE_REMOVE).toBool();

    // Allow cursor around
    m_allowCursorAround = value("main/allowCursorAround", DEFAULT_ALLOW_CURSOR_AROUND).toBool();

    // Move the cursor to the next, after open the viewer with Enter key
    m_moveCursorOpenViewer = value("main/moveCursorOpenViewer", DEFAULT_MOVE_CURSOR_OPEN_VIEWER).toBool();

    // Confirm quit
    m_confirmQuit = value("main/confirmQuit", DEFAULT_CONFIRM_QUIT).toBool();

    // File size format (Single pane)
    QString singlePaneFileSizeFormatTypeValue = value("main/singlePaneFileSizeFormatType").toString();
    m_singlePaneFileSizeFormatType = (singlePaneFileSizeFormatTypeValue == "si")     ? FileSizeFormatType::SI :
                                     (singlePaneFileSizeFormatTypeValue == "iec")    ? FileSizeFormatType::IEC :
                                     (singlePaneFileSizeFormatTypeValue == "detail") ? FileSizeFormatType::Detail :
                                                                                       DEFAULT_SINGLE_PANE_FILE_SIZE_FORMAT_TYPE;
    m_singlePaneFileSizeDetailCommaEnable = value("main/singlePaneFileSizeDetailCommaEnable", DEFAULT_SINGLE_PANE_FILE_SIZE_DETAIL_COMMA_ENABLE).toBool();

    // File size format (Dual pane)
    QString dualPaneFileSizeFormatTypeValue = value("main/dualPaneFileSizeFormatType").toString();
    m_dualPaneFileSizeFormatType = (dualPaneFileSizeFormatTypeValue == "si")     ? FileSizeFormatType::SI :
                                   (dualPaneFileSizeFormatTypeValue == "iec")    ? FileSizeFormatType::IEC :
                                   (dualPaneFileSizeFormatTypeValue == "detail") ? FileSizeFormatType::Detail :
                                                                                   DEFAULT_DUAL_PANE_FILE_SIZE_FORMAT_TYPE;
    m_dualPaneFileSizeDetailCommaEnable = value("main/dualPaneFileSizeDetailCommaEnable", DEFAULT_DUAL_PANE_FILE_SIZE_DETAIL_COMMA_ENABLE).toBool();

    // Date format (Single pane)
    QString singlePaneDateFormatTypeValue = value("main/singlePaneDateFormatType").toString();
    m_singlePaneDateFormatType = (singlePaneDateFormatTypeValue == "default")  ? DateFormatType::Default :
                                 (singlePaneDateFormatTypeValue == "iso")      ? DateFormatType::ISO :
                                 (singlePaneDateFormatTypeValue == "original") ? DateFormatType::Original :
                                                                                 DEFAULT_SINGLE_PANE_DATE_FORMAT_TYPE;
    m_singlePaneDateFormatOriginalString = value("main/singlePaneDateFormatOriginalString", DEFAULT_SINGLE_PANE_DATE_FORMAT_ORIGINAL_STRING).toString();

    // Date format (Dual pane)
    QString dualPaneDateFormatTypeValue = value("main/dualPaneDateFormatType").toString();
    m_dualPaneDateFormatType = (dualPaneDateFormatTypeValue == "default")  ? DateFormatType::Default :
                               (dualPaneDateFormatTypeValue == "iso")      ? DateFormatType::ISO :
                               (dualPaneDateFormatTypeValue == "original") ? DateFormatType::Original :
                                                                             DEFAULT_DUAL_PANE_DATE_FORMAT_TYPE;
    m_dualPaneDateFormatOriginalString = value("main/dualPaneDateFormatOriginalString", DEFAULT_DUAL_PANE_DATE_FORMAT_ORIGINAL_STRING).toString();

    // Color settings
    for(auto colorSettingKey : m_colorSettings.keys())
    {
        m_colorSettings[colorSettingKey] = getValueColorSetting("main/color/" + colorSettingKey, DEFAULT_COLOR_SETTINGS[colorSettingKey]);
    }

    // Font settings
    for(auto fontSettingKey : m_fontSettings.keys())
    {
        QString fontSettingString = value("main/font/" + fontSettingKey).toString();
        QFont fontSettingValue = QFont();
        if(fontSettingString.length() > 0 && fontSettingValue.fromString(fontSettingString))
        {
            m_fontSettings[fontSettingKey] = fontSettingValue;
        }
        else
        {
            m_fontSettings[fontSettingKey] = DEFAULT_FONT_SETTINGS[fontSettingKey];
        }
    }

    // Cursor width
    m_cursorWidth = value("main/cursorWidth", QString("%1").arg(DEFAULT_CURSOR_WIDTH)).toInt();

    // Console visible
    m_consoleVisible = value("main/consoleVisible", DEFAULT_CONSOLE_VISIBLE).toBool();

    // TextViewer
    {
        // Show line number
        m_textViewerShowLineNumber = value("main/textViewer_showLineNumber", DEFAULT_TEXT_VIEWER_SHOW_LINE_NUMBER).toBool();

        // Word wrap
        m_textViewerWordWrap = value("main/textViewer_wordWrap", DEFAULT_TEXT_VIEWER_WORD_WRAP).toBool();

        // Encode list
        m_textViewerEncodeList.clear();
        int size = beginReadArray("main/textViewer_encodeList");
        if(size > 0)
        {
            for(int i = 0;i < size;i++)
            {
                setArrayIndex(i);
                m_textViewerEncodeList.append(value("encode").toString());
            }
        }
        else
        {
            for(QByteArray encode : QTextCodec::availableCodecs())
            {
                m_textViewerEncodeList.append(QString(encode));
            }
        }
        endArray();
    }

    // ImageViewer
    {
        // Fit in view
        m_imageViewerFitInView = value("main/imageViewer_fitInView", DEFAULT_IMAGE_VIEWER_FIT_IN_VIEW).toBool();

        // BG type
        QString imageViewerTransparentBGTypeValue = value("main/imageViewer_transparentBgType", "checkered").toString();
        m_imageViewerTransparentBGType = (imageViewerTransparentBGTypeValue == "solid")     ? ImageViewerTransparentBGType::Solid :
                                         (imageViewerTransparentBGTypeValue == "checkered") ? ImageViewerTransparentBGType::Checkered :
                                                                                              DEFAULT_IMAGE_VIEWER_TRANSPARENT_BG_TYPE;
    }

    // External Application
    m_textEditorPath = value("main/textEditorPath", DEFAULT_TEXT_EDITOR_PATH).toString();
    m_textEditorArgs = value("main/textEditorArgs", DEFAULT_TEXT_EDITOR_ARGS).toString();
}

void Settings::flush()
{
    // Copy unit size
    setValue("main/copyUnitSize", m_copyUnitSize);

    // Size at startup
    QString sizeAtStartupTypeValue = (m_sizeAtStartupType == SizeAtStartup::LastTime) ? "lastTime" :
                                     (m_sizeAtStartupType == SizeAtStartup::Fixed)    ? "fixed" :
                                                                                        "default";
    setValue("main/sizeAtStartupType", sizeAtStartupTypeValue);
    setValue("main/sizeAtStartup", m_sizeAtStartup);

    // Position at startup
    QString positionAtStartupTypeValue = (m_positionAtStartupType == PositionAtStartup::LastTime) ? "lastTime" :
                                         (m_positionAtStartupType == PositionAtStartup::Fixed)    ? "fixed" :
                                                                                                    "default";
    setValue("main/positionAtStartupType", positionAtStartupTypeValue);
    setValue("main/positionAtStartup", m_positionAtStartup);

    // Pane
    QString paneModeValue = (m_paneMode == PaneMode::Single) ? "single" :
                                                               "dual";
    setValue("main/paneMode", paneModeValue);

    QString activePaneValue = (m_activePane == PaneType::Right) ? "right" :
                                                                  "left";
    setValue("main/activePane", activePaneValue);

    // Folder at startup(Left)
    QString leftFolderAtStartupValue = (m_leftFolderAtStartup == FolderAtStartup::LastTime) ? "lastTime" :
                                       (m_leftFolderAtStartup == FolderAtStartup::Fixed)    ? "fixed" :
                                                                                              "default";
    setValue("main/leftFolderAtStartup", leftFolderAtStartupValue);
    setValue("main/leftFolderPath", m_leftFolderPath);

    // Folder at startup(Right)
    QString rightFolderAtStartupValue = (m_rightFolderAtStartup == FolderAtStartup::LastTime) ? "lastTime" :
                                        (m_rightFolderAtStartup == FolderAtStartup::Fixed)    ? "fixed" :
                                                                                                "default";
    setValue("main/rightFolderAtStartup", rightFolderAtStartupValue);
    setValue("main/rightFolderPath", m_rightFolderPath);

    // Left side Sort settings
    setValueSortSectionType(m_leftSortSectionType, "left");
    setValueSortDirsType(m_leftSortDirsType, "left");
    setValueSortDotFirst(m_leftSortDotFirst, "left");
    setValueSortCaseSensitivity(m_leftSortCaseSensitivity, "left");
    setValueSortOrder(m_leftSortOrder, "left");

    // Right side Sort settings
    setValueSortSectionType(m_rightSortSectionType, "right");
    setValueSortDirsType(m_rightSortDirsType, "right");
    setValueSortDotFirst(m_rightSortDotFirst, "right");
    setValueSortCaseSensitivity(m_rightSortCaseSensitivity, "right");
    setValueSortOrder(m_rightSortOrder, "right");

    // Left side Filter settings
    setValueFilterSettings(m_leftFilterSettings, "left");

    // Right side Filter settings
    setValueFilterSettings(m_rightFilterSettings, "right");

    // Drag & Drop behavior in FolderView
    QString behaviorTypeValue = (m_dragAndDropBehaviorType == DragAndDropBehaviorType::Copy) ? "copy" :
                                (m_dragAndDropBehaviorType == DragAndDropBehaviorType::Move) ? "move" :
                                                                                               "select";
    setValue("main/dragAndDropBehaviorType", behaviorTypeValue);

    // Auto dialog closing at successful operation.
    setValue("main/autoDialogCloseCopy", m_autoDialogCloseCopy);
    setValue("main/autoDialogCloseMove", m_autoDialogCloseMove);
    setValue("main/autoDialogCloseRemove", m_autoDialogCloseRemove);

    // Allow cursor around
    setValue("main/allowCursorAround", m_allowCursorAround);

    // Move the cursor to the next, after open the viewer with Enter key.
    setValue("main/moveCursorOpenViewer", m_moveCursorOpenViewer);

    // Confirm at quit
    setValue("main/confirmQuit", m_confirmQuit);

    // Data size format (Single pane)
    QString singlePaneFileSizeFormatTypeValue = (m_singlePaneFileSizeFormatType == FileSizeFormatType::IEC)    ? "iec" :
                                                (m_singlePaneFileSizeFormatType == FileSizeFormatType::Detail) ? "detail" :
                                                                                                                 "si";
    setValue("main/singlePaneFileSizeFormatType", singlePaneFileSizeFormatTypeValue);
    setValue("main/singlePaneFileSizeDetailCommaEnable", m_singlePaneFileSizeDetailCommaEnable);

    // Data size format (Dual pane)
    QString dualPaneFileSizeFormatTypeValue = (m_dualPaneFileSizeFormatType == FileSizeFormatType::IEC)    ? "iec" :
                                              (m_dualPaneFileSizeFormatType == FileSizeFormatType::Detail) ? "detail" :
                                                                                                             "si";
    setValue("main/dualPaneFileSizeFormatType", dualPaneFileSizeFormatTypeValue);
    setValue("main/dualPaneFileSizeDetailCommaEnable", m_dualPaneFileSizeDetailCommaEnable);

    // Date format (Single pane)
    QString singlePaneDateFormatTypeValue = (m_singlePaneDateFormatType == DateFormatType::ISO)      ? "iso" :
                                            (m_singlePaneDateFormatType == DateFormatType::Original) ? "original" :
                                                                                                       "default";
    setValue("main/singlePaneDateFormatType", singlePaneDateFormatTypeValue);
    setValue("main/singlePaneDateFormatOriginalString", m_singlePaneDateFormatOriginalString);

    // Date format (Dual pane)
    QString dualPaneDateFormatTypeValue = (m_dualPaneDateFormatType == DateFormatType::ISO)      ? "iso" :
                                          (m_dualPaneDateFormatType == DateFormatType::Original) ? "original" :
                                                                                                   "default";
    setValue("main/dualPaneDateFormatType", dualPaneDateFormatTypeValue);
    setValue("main/dualPaneDateFormatOriginalString", m_dualPaneDateFormatOriginalString);

    // Color settings
    for(auto colorSettingKey : m_colorSettings.keys())
    {
        const QColor& colorSettingValue = m_colorSettings[colorSettingKey];
        setValue("main/color/" + colorSettingKey, colorSettingValue.name());
    }

    // Font settings
    for(auto fontSettingKey : m_fontSettings.keys())
    {
        const QFont& fontSettingValue = m_fontSettings[fontSettingKey];
        setValue("main/font/" + fontSettingKey, fontSettingValue.toString());
    }

    // Cursor width
    setValue("main/cursorWidth", m_cursorWidth);

    // Console visible
    setValue("main/consoleVisible", m_consoleVisible);

    // Text Viewer
    {
        // Show line number
        setValue("main/textViewer_showLineNumber", m_textViewerShowLineNumber);

        // Word wrap
        setValue("main/textViewer_wordWrap", m_textViewerWordWrap);

        // Encode list
        beginWriteArray("main/textViewer_encodeList");
        for(int i = 0;i < m_textViewerEncodeList.size();i++)
        {
            setArrayIndex(i);
            setValue("encode", m_textViewerEncodeList[i]);
        }
        endArray();
    }

    // Image Viewer
    {
        // Fit in view
        setValue("main/imageViewer_fitInView", m_imageViewerFitInView);

        // BG type
        QString imageViewerTransparentBGTypeValue = (m_imageViewerTransparentBGType == ImageViewerTransparentBGType::Solid) ? "solid" :
                                                                                                                              "checkered";
        setValue("main/imageViewer_transparentBgType", imageViewerTransparentBGTypeValue);
    }

    // External Application
    setValue("main/textEditorPath", m_textEditorPath);
    setValue("main/textEditorArgs", m_textEditorArgs);
}

QColor Settings::getValueColorSetting(const QString& key, const QColor& defColor)
{
    QString colorSettingString = value(key).toString();
    if(colorSettingString.length() > 0 && QColor::isValidColor(colorSettingString))
    {
        return QColor(colorSettingString);
    }
    else
    {
        return defColor;
    }
}

QColor Settings::getColorSetting(const QString& colorSettingType)
{
    QColor ret;

    QMap<QString, QColor>::const_iterator itr = m_colorSettings.find(colorSettingType);
    if(itr != m_colorSettings.end())
    {
        ret = *itr;
    }

    return ret;
}

void Settings::setColorSetting(const QString& colorSettingType, const QColor& color)
{
    m_colorSettings[colorSettingType] = color;
}

QFont Settings::getFontSetting(const QString& fontSettingType)
{
    QFont ret;

    QMap<QString, QFont>::const_iterator itr = m_fontSettings.find(fontSettingType);
    if(itr != m_fontSettings.end())
    {
        ret = *itr;
    }

    return ret;
}

void Settings::setFontSetting(const QString& fontSettingType, const QFont& font)
{
    m_fontSettings[fontSettingType] = font;
}

SectionType Settings::getValueSortSectionType(const QString& prefix)
{
    SectionType ret = DEFAULT_SORT_SECTION_TYPE;

    QString sortTypeValue = value("main/" + prefix + "SortType").toString();
    ret = (sortTypeValue == "lastModified") ? SectionType::LastModified :
          (sortTypeValue == "size") ? SectionType::FileSize :
          (sortTypeValue == "type") ? SectionType::FileType :
          (sortTypeValue == "name") ? SectionType::FileName :
                                      DEFAULT_SORT_SECTION_TYPE;

    return ret;
}

SortDirsType Settings::getValueSortDirsType(const QString& prefix)
{
    SortDirsType ret = DEFAULT_SORT_DIRS_TYPE;

    QString sortDirsValue = value("main/" + prefix + "SortDirs").toString();
    ret = (sortDirsValue == "first") ? SortDirsType::First :
          (sortDirsValue == "last")  ? SortDirsType::Last :
          (sortDirsValue == "none")  ? SortDirsType::NoSpecify :
                                       DEFAULT_SORT_DIRS_TYPE;

    return ret;
}

bool Settings::getValueSortDotFirst(const QString& prefix)
{
    return value("main/" + prefix + "SortDotFirst", DEFAULT_SORT_DOT_FIRST).toBool();
}

Qt::CaseSensitivity Settings::getValueSortCaseSensitivity(const QString& prefix)
{
    Qt::CaseSensitivity ret = DEFAULT_SORT_CASE_SENSITIVITY;

    QString sortCaseValue = value("main/" + prefix + "SortCase").toString();
    ret = (sortCaseValue == "insensitive") ? Qt::CaseInsensitive :
          (sortCaseValue == "sensitive")   ? Qt::CaseSensitive :
                                             DEFAULT_SORT_CASE_SENSITIVITY;

    return ret;
}

Qt::SortOrder Settings::getValueSortOrder(const QString& prefix)
{
    Qt::SortOrder ret = Qt::AscendingOrder;

    QString sortOrderValue = value("main/" + prefix + "SortOrder").toString();
    ret = (sortOrderValue == "asc")  ? Qt::AscendingOrder :
          (sortOrderValue == "desc") ? Qt::DescendingOrder :
                                       DEFAULT_SORT_ORDER;

    return ret;
}

void Settings::setValueSortSectionType(SectionType sectionType, const QString& prefix)
{
    QString sortTypeValue = (sectionType == SectionType::LastModified) ? "lastModified" :
                            (sectionType == SectionType::FileSize) ? "size" :
                            (sectionType == SectionType::FileType) ? "type" :
                                                                     "name";
    setValue("main/" + prefix + "SortType", sortTypeValue);
}

void Settings::setValueSortDirsType(SortDirsType dirsType, const QString& prefix)
{
    QString sortDirsValue = (dirsType == SortDirsType::First) ? "first" :
                            (dirsType == SortDirsType::Last) ? "last" :
                                                               "none";
    setValue("main/" + prefix + "SortDirs", sortDirsValue);
}

void Settings::setValueSortDotFirst(bool dotFirst, const QString& prefix)
{
    setValue("main/" + prefix + "SortDotFirst", dotFirst);
}

void Settings::setValueSortCaseSensitivity(Qt::CaseSensitivity caseSensitivity, const QString& prefix)
{
    QString sortCaseValue = (caseSensitivity == Qt::CaseInsensitive) ? "insensitive" :
                                                                       "sensitive";
    setValue("main/" + prefix + "SortCase", sortCaseValue);
}

void Settings::setValueSortOrder(Qt::SortOrder order, const QString& prefix)
{
    QString sortOrderValue = (order == Qt::DescendingOrder) ? "desc" :
                                                              "asc";
    setValue("main/" + prefix + "SortOrder", sortOrderValue);
}

QDir::Filters Settings::getValueFilterSettings(const QString& prefix)
{
    QDir::Filters ret = DEFAULT_FILTER_FLAGS;

    if(value("main/" + prefix + "FilterHidden", false).toBool())
    {
        ret |= QDir::Filter::Hidden;
    }

    if(value("main/" + prefix + "FilterSystem", false).toBool())
    {
        ret |= QDir::Filter::System;
    }

    return ret;
}

void Settings::setValueFilterSettings(QDir::Filters filterSettings, const QString& prefix)
{
    bool filterHiddenValue = (filterSettings & QDir::Filter::Hidden) ? true : false;
    setValue("main/" + prefix + "FilterHidden", filterHiddenValue);

    bool filterSystemValue = (filterSettings & QDir::Filter::System) ? true : false;
    setValue("main/" + prefix + "FilterSystem", filterSystemValue);
}

}
