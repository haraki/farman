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
    // Size at startup
    QString sizeAtStartupTypeValue = value("main/sizeAtStartupType", "default").toString();
    m_sizeAtStartupType = (sizeAtStartupTypeValue == "lastTime") ? SizeAtStartup::LastTime :
                          (sizeAtStartupTypeValue == "fixed") ? SizeAtStartup::Fixed :
                                                                SizeAtStartup::Default;
    m_sizeAtStartup = value("main/sizeAtStartup", QSize(0, 0)).toSize();

    // Position at startup
    QString positionAtStartupTypeValue = value("main/positionAtStartupType", "default").toString();
    m_positionAtStartupType = (positionAtStartupTypeValue == "lastTime") ? PositionAtStartup::LastTime :
                              (positionAtStartupTypeValue == "fixed") ? PositionAtStartup::Fixed :
                                                                        PositionAtStartup::Default;
    m_positionAtStartup = value("main/positionAtStartup", QPoint(0, 0)).toPoint();

    // View mode
    QString viewModeValue = value("main/viewMode", "double").toString();
    m_viewMode = (viewModeValue == "single") ? ViewMode::Single : ViewMode::Double;

    // Folder at startup(Left)
    QString leftFolderAtStartupValue = value("main/leftFolderAtStartup", "default").toString();
    m_leftFolderAtStartup = (leftFolderAtStartupValue == "lastTime") ? FolderAtStartup::LastTime :
                            (leftFolderAtStartupValue == "fixed") ? FolderAtStartup::Fixed :
                                                                    FolderAtStartup::Default;
    m_leftFolderPath = value("main/leftFolderPath", QString("")).toString();

    // Folder at startup(Right)
    QString rightFolderAtStartupValue = value("main/rightFolderAtStartup", "default").toString();
    m_rightFolderAtStartup = (rightFolderAtStartupValue == "lastTime") ? FolderAtStartup::LastTime :
                             (rightFolderAtStartupValue == "fixed") ? FolderAtStartup::Fixed :
                                                                      FolderAtStartup::Default;
    m_rightFolderPath = value("main/rightFolderPath", QString("")).toString();

    // Left side Sort settings
    m_leftSortSectionType = getSortSectionType("left");
    m_leftSortDirsType = getSortDirsType("left");
    m_leftSortDotFirst = getSortDotFirst("left");
    m_leftSortCaseSensitivity = getSortCaseSensitivity("left");
    m_leftSortOrder = getSortOrder("left");

    // Right side Sort settings
    m_rightSortSectionType = getSortSectionType("right");
    m_rightSortDirsType = getSortDirsType("right");
    m_rightSortDotFirst = getSortDotFirst("right");
    m_rightSortCaseSensitivity = getSortCaseSensitivity("right");
    m_rightSortOrder = getSortOrder("right");

    // Left side Filter settings
    m_leftFilterSettings = getFilterSettings("left");

    // Right side Filter settings
    m_rightFilterSettings = getFilterSettings("right");

    // Drag & Drop behavior in FolderView
    QString behaviorTypeValue = value("main/dragAndDropBehaviorType", "select").toString();
    m_dragAndDropBehaviorType = (behaviorTypeValue == "copy") ? DragAndDropBehaviorType::Copy :
                                (behaviorTypeValue == "move") ? DragAndDropBehaviorType::Move :
                                                                DragAndDropBehaviorType::Select;

    // Auto dialog closing at successful operation.
    m_autoDialogCloseCopy = value("main/autoDialogCloseCopy", false).toBool();
    m_autoDialogCloseMove = value("main/autoDialogCloseMove", false).toBool();
    m_autoDialogCloseRemove = value("main/autoDialogCloseRemove", false).toBool();

    // Confirm quit
    QString confirmQuitValue = value("main/confirmQuit", "true").toString();
    m_confirmQuit = (confirmQuitValue == "false") ? false : true;

    auto getColorSettingParam = [this](const QString& key, const QColor& defColor)
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
    };

    // Color settings
    for(auto colorSettingKey : m_colorSettings.keys())
    {
        m_colorSettings[colorSettingKey] = getColorSettingParam("main/color/" + colorSettingKey, m_defaultColorSettings[colorSettingKey]);
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
            m_fontSettings[fontSettingKey] = m_defaultFontSettings[fontSettingKey];
        }
    }

    // Cursor width
    m_cursorWidth = value("main/cursorWidth", QString("%1").arg(DEFAULT_CURSOR_WIDTH)).toInt();

    // ImageViewer
    {
        // Fit in view
        QString imageViewerFitInViewValue = value("main/imageViewer_fitInView", "true").toString();
        m_imageViewerFitInView = (imageViewerFitInViewValue == "false") ? false : true;

        // BG type
        QString imageViewerBGTypeValue = value("main/imageViewer_bgType", "checkered").toString();
        m_imageViewerBGType = (imageViewerBGTypeValue == "solid") ? ImageViewerBGType::Solid : ImageViewerBGType::Checkered;
    }

    // TextViewer
    {
        // Show line number
        QString textViewerShowLineNumberValue = value("main/textViewer_showLineNumber", "true").toString();
        m_textViewerShowLineNumber = (textViewerShowLineNumberValue == "false") ? false : true;

        // Word wrap
        QString textViewerWordWrapValue = value("main/textViewer_wordWrap", "false").toString();
        m_textViewerWordWrap = (textViewerWordWrapValue == "true") ? true : false;

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
}

void Settings::flush()
{
    // Size at startup
    QString sizeAtStartupTypeValue = (m_sizeAtStartupType == SizeAtStartup::LastTime) ? "lastTime" :
                                     (m_sizeAtStartupType == SizeAtStartup::Fixed) ? "fixed" :
                                                                                     "default";
    setValue("main/sizeAtStartupType", sizeAtStartupTypeValue);
    setValue("main/sizeAtStartup", m_sizeAtStartup);

    // Position at startup
    QString positionAtStartupTypeValue = (m_positionAtStartupType == PositionAtStartup::LastTime) ? "lastTime" :
                                         (m_positionAtStartupType == PositionAtStartup::Fixed) ? "fixed" :
                                                                                                 "default";
    setValue("main/positionAtStartupType", positionAtStartupTypeValue);
    setValue("main/positionAtStartup", m_positionAtStartup);

    // View mode
    QString viewModeValue = (m_viewMode == ViewMode::Single) ? "single"
                                                             : "double";
    setValue("main/viewMode", viewModeValue);

    // Folder at startup(Left)
    QString leftFolderAtStartupValue = (m_leftFolderAtStartup == FolderAtStartup::LastTime) ? "lastTime" :
                                       (m_leftFolderAtStartup == FolderAtStartup::Fixed) ? "fixed" :
                                                                                           "default";
    setValue("main/leftFolderAtStartup", leftFolderAtStartupValue);
    setValue("main/leftFolderPath", m_leftFolderPath);

    // Folder at startup(Right)
    QString rightFolderAtStartupValue = (m_rightFolderAtStartup == FolderAtStartup::LastTime) ? "lastTime" :
                                        (m_rightFolderAtStartup == FolderAtStartup::Fixed) ? "fixed" :
                                                                                             "default";
    setValue("main/rightFolderAtStartup", rightFolderAtStartupValue);
    setValue("main/rightFolderPath", m_rightFolderPath);

    // Left side Sort settings
    setSortSectionType(m_leftSortSectionType, "left");
    setSortDirsType(m_leftSortDirsType, "left");
    setSortDotFirst(m_leftSortDotFirst, "left");
    setSortCaseSensitivity(m_leftSortCaseSensitivity, "left");
    setSortOrder(m_leftSortOrder, "left");

    // Right side Sort settings
    setSortSectionType(m_rightSortSectionType, "right");
    setSortDirsType(m_rightSortDirsType, "right");
    setSortDotFirst(m_rightSortDotFirst, "right");
    setSortCaseSensitivity(m_rightSortCaseSensitivity, "right");
    setSortOrder(m_rightSortOrder, "right");

    // Left side Filter settings
    setFilterSettings(m_leftFilterSettings, "left");

    // Right side Filter settings
    setFilterSettings(m_rightFilterSettings, "right");

    // Drag & Drop behavior in FolderView
    QString behaviorTypeValue = (m_dragAndDropBehaviorType == DragAndDropBehaviorType::Copy) ? "copy" :
                                (m_dragAndDropBehaviorType == DragAndDropBehaviorType::Move) ? "move" :
                                                                                               "select";
    setValue("main/dragAndDropBehaviorType", behaviorTypeValue);

    // Auto dialog closing at successful operation.
    setValue("main/autoDialogCloseCopy", m_autoDialogCloseCopy);
    setValue("main/autoDialogCloseMove", m_autoDialogCloseMove);
    setValue("main/autoDialogCloseRemove", m_autoDialogCloseRemove);

    // Confirm at quit
    setValue("main/confirmQuit", m_confirmQuit);

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

    // Image Viewer
    {
        // Fit in view
        setValue("main/imageViewer_fitInView", m_imageViewerFitInView);

        // BG type
        QString imageViewerBGTypeValue = (m_imageViewerBGType == ImageViewerBGType::Solid) ? "solid"
                                                                                           : "checkered";
        setValue("main/imageViewer_bgType", imageViewerBGTypeValue);
    }

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

SectionType Settings::getSortSectionType(const QString& prefix)
{
    SectionType ret = SectionType::FileName;

    QString sortTypeValue = value("main/" + prefix + "SortType", "name").toString();
    ret = (sortTypeValue == "lastModified") ? SectionType::LastModified :
          (sortTypeValue == "size") ? SectionType::FileSize :
          (sortTypeValue == "type") ? SectionType::FileType :
                                      SectionType::FileName;

    return ret;
}

SortDirsType Settings::getSortDirsType(const QString& prefix)
{
    SortDirsType ret = SortDirsType::NoSpecify;

    QString sortDirsValue = value("main/" + prefix + "SortDirs", "first").toString();
    ret = (sortDirsValue == "first") ? SortDirsType::First :
          (sortDirsValue == "last") ? SortDirsType::Last :
                                      SortDirsType::NoSpecify;

    return ret;
}

bool Settings::getSortDotFirst(const QString& prefix)
{
    bool ret = true;

    QString sortDotFirstValue = value("main/" + prefix + "SortDotFirst", "true").toString();
    ret = (sortDotFirstValue == "false") ? false :
                                           true;

    return ret;
}

Qt::CaseSensitivity Settings::getSortCaseSensitivity(const QString& prefix)
{
    Qt::CaseSensitivity ret = Qt::CaseSensitive;

    QString sortCaseValue = value("main/" + prefix + "SortCase", "sensitive").toString();
    ret = (sortCaseValue == "insensitive") ? Qt::CaseInsensitive :
                                             Qt::CaseSensitive;

    return ret;
}

Qt::SortOrder Settings::getSortOrder(const QString& prefix)
{
    Qt::SortOrder ret = Qt::AscendingOrder;

    QString sortOrderValue = value("main/" + prefix + "SortOrder", "asc").toString();
    ret = (sortOrderValue == "desc") ? Qt::DescendingOrder :
                                       Qt::AscendingOrder;

    return ret;
}

void Settings::setSortSectionType(SectionType sectionType, const QString& prefix)
{
    QString sortTypeValue = (sectionType == SectionType::LastModified) ? "lastModified" :
                            (sectionType == SectionType::FileSize) ? "size" :
                            (sectionType == SectionType::FileType) ? "type" :
                                                                     "name";
    setValue("main/" + prefix + "SortType", sortTypeValue);
}

void Settings::setSortDirsType(SortDirsType dirsType, const QString& prefix)
{
    QString sortDirsValue = (dirsType == SortDirsType::First) ? "first" :
                            (dirsType == SortDirsType::Last) ? "last" :
                                                               "none";
    setValue("main/" + prefix + "SortDirs", sortDirsValue);
}

void Settings::setSortDotFirst(bool dotFirst, const QString& prefix)
{
    QString sortDotFirstValue = (dotFirst == false) ? "false" :
                                                      "true";
    setValue("main/" + prefix + "SortDotFirst", sortDotFirstValue);
}

void Settings::setSortCaseSensitivity(Qt::CaseSensitivity caseSensitivity, const QString& prefix)
{
    QString sortCaseValue = (caseSensitivity == Qt::CaseInsensitive) ? "insensitive" :
                                                                       "sensitive";
    setValue("main/" + prefix + "SortCase", sortCaseValue);
}

void Settings::setSortOrder(Qt::SortOrder order, const QString& prefix)
{
    QString sortOrderValue = (order == Qt::DescendingOrder) ? "desc" :
                                                              "asc";
    setValue("main/" + prefix + "SortOrder", sortOrderValue);
}

QDir::Filters Settings::getFilterSettings(const QString& prefix)
{
    QDir::Filters ret = FIX_FILTER_FLAGS;

    QString filterHiddenValue = value("main/" + prefix + "FilterHidden", "false").toString();
    ret |= (filterHiddenValue == "true") ? QDir::Filter::Hidden :
                                           static_cast<QDir::Filters>(0);

    QString filterSystemValue = value("main/" + prefix + "FilterSystem", "false").toString();
    ret |= (filterSystemValue == "true") ? QDir::Filter::System :
                                           static_cast<QDir::Filters>(0);

    return ret;
}

void Settings::setFilterSettings(QDir::Filters filterSettings, const QString& prefix)
{
    QString filterHiddenValue = (filterSettings & QDir::Filter::Hidden) ? "true" :
                                                                          "false";
    setValue("main/" + prefix + "FilterHidden", filterHiddenValue);

    QString filterSystemValue = (filterSettings & QDir::Filter::System) ? "true" :
                                                                          "false";
    setValue("main/" + prefix + "FilterSystem", filterSystemValue);
}

}
