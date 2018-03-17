#include <QTextCodec>
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
    : QSettings("settings.ini", QSettings::Format::IniFormat)
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
    m_leftSortSettings = getSortSettings("left");

    // Right side Sort settings
    m_rightSortSettings = getSortSettings("right");

    // Left side Filter settings
    m_leftFilterSettings = getFilterSettings("left");

    // Right side Filter settings
    m_rightFilterSettings = getFilterSettings("right");

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

    // ImageViewer Fit in view
    QString imageViewerFitInViewValue = value("main/imageViewer_fitInView", "true").toString();
    m_imageViewerFitInView = (imageViewerFitInViewValue == "false") ? false : true;

    // ImageViewer BG type
    QString imageViewerBGTypeValue = value("main/imageViewer_bgType", "checkered").toString();
    m_imageViewerBGType = (imageViewerBGTypeValue == "solid") ? ImageViewerBGType::Solid : ImageViewerBGType::Checkered;

    // TextViewer Show line number
    QString textViewerShowLineNumberValue = value("main/textViewer_showLineNumber", "true").toString();
    m_textViewerShowLineNumber = (textViewerShowLineNumberValue == "false") ? false : true;

    // TextViewer Word wrap
    QString textViewerWordWrapValue = value("main/textViewer_wordWrap", "false").toString();
    m_textViewerWordWrap = (textViewerWordWrapValue == "true") ? true : false;

    // TextViewer Encode list
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
    setSortSettings(m_leftSortSettings, "left");

    // Right side Sort settings
    setSortSettings(m_rightSortSettings, "right");

    // Left side Filter settings
    setFilterSettings(m_leftFilterSettings, "left");

    // Right side Filter settings
    setFilterSettings(m_rightFilterSettings, "right");

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

    // ImageViewer Fit in view
    setValue("main/imageViewer_fitInView", m_imageViewerFitInView);

    // ImageViewer BG type
    QString imageViewerBGTypeValue = (m_imageViewerBGType == ImageViewerBGType::Solid) ? "solid"
                                                                                       : "checkered";
    setValue("main/imageViewer_bgType", imageViewerBGTypeValue);

    // TextViewer Show line number
    setValue("main/textViewer_showLineNumber", m_textViewerShowLineNumber);

    // TextViewer Word wrap
    setValue("main/textViewer_wordWrap", m_textViewerWordWrap);

    // TextViewer Encode list
    beginWriteArray("main/textViewer_encodeList");
    for(int i = 0;i < m_textViewerEncodeList.size();i++)
    {
        setArrayIndex(i);
        setValue("encode", m_textViewerEncodeList[i]);
    }
    endArray();
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

QDir::SortFlags Settings::getSortSettings(const QString& prefix)
{
    QDir::SortFlags ret = FIX_SORT_FLAGS;

    QString sortTypeValue = value("main/" + prefix + "SortType", "name").toString();
    ret |= (sortTypeValue == "lastModified") ? QDir::SortFlag::Time :
           (sortTypeValue == "size") ? QDir::SortFlag::Size :
           (sortTypeValue == "type") ? QDir::SortFlag::Type :
                                       QDir::SortFlag::Name;

    QString sortOrderValue = value("main/" + prefix + "SortOrder", "asc").toString();
    ret |= (sortOrderValue == "desc") ? QDir::SortFlag::Reversed :
                                        static_cast<QDir::SortFlags>(0);

    QString sortDirsValue = value("main/" + prefix + "SortDirs", "first").toString();
    ret |= (sortDirsValue == "first") ? QDir::SortFlag::DirsFirst :
           (sortDirsValue == "last") ? QDir::SortFlag::DirsLast :
                                       static_cast<QDir::SortFlags>(0);

    QString sortCaseValue = value("main/" + prefix + "SortCase", "sensitive").toString();
    ret |= (sortCaseValue == "insensitive") ? QDir::SortFlag::IgnoreCase :
                                              static_cast<QDir::SortFlags>(0);

    return ret;
}

void Settings::setSortSettings(QDir::SortFlags sortSettings, const QString& prefix)
{
    QString sortTypeValue = (sortSettings & QDir::SortFlag::Type) ? "type" :
                            ((sortSettings & QDir::SortFlag::SortByMask) == QDir::SortFlag::Time) ? "lastModified" :
                            ((sortSettings & QDir::SortFlag::SortByMask) == QDir::SortFlag::Size) ? "size" :
                                                                                                    "name";
    setValue("main/" + prefix + "SortType", sortTypeValue);

    QString sortOrderValue = (sortSettings & QDir::SortFlag::Reversed) ? "desc" :
                                                                         "asc";
    setValue("main/" + prefix + "SortOrder", sortOrderValue);

    QString sortDirsValue = (sortSettings & QDir::SortFlag::DirsFirst) ? "first" :
                            (sortSettings & QDir::SortFlag::DirsLast) ? "last" :
                                                                        "none";
    setValue("main/" + prefix + "SortDirs", sortDirsValue);

    QString sortCaseValue = (sortSettings & QDir::SortFlag::IgnoreCase) ? "insensitive" :
                                                                          "sensitive";
    setValue("main/" + prefix + "SortCase", sortCaseValue);
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
