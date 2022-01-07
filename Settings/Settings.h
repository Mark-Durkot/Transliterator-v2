#ifndef SETTINGS_H
#define SETTINGS_H

#include <QRect>
#include <QFont>
#include <QSettings>
#include <QGuiApplication>
#include <QScreen>
#include <QColor>
#include <QDebug>

class Settings
{
public:
    Settings(const QString &organizationName, const QString &applicationName)
        : settings(organizationName, applicationName)
    {
    }

    void saveMainWindowGeometry(const QRect &geometry)
    {
        settings.beginGroup("mainwindow_geometry");

        settings.setValue("position", geometry.topLeft());
        settings.setValue("size", geometry.size());

        settings.endGroup();
    }

    QRect loadMainWindowGeometry()
    {
        settings.beginGroup("mainwindow_geometry");

        auto pos  = settings.value("position", getCenterOfScreen()).toPoint();
        auto size = settings.value("size", getDefaultMainWindowSize()).toSize();

        settings.endGroup();

        return QRect(pos, size);
    }

    QRect getDefaultMainWindowGeometry()
    {
        return { getCenterOfScreen(), getDefaultMainWindowSize() };
    }

    QPoint getCenterOfScreen()
    {
        return QGuiApplication::screens().at(0)->geometry().center();
    }

    QSize getDefaultMainWindowSize()
    {
        return { 800, 264 };  // temporary default size
    }

    void saveFont(const QFont &font)
    {
        settings.beginGroup("mainwindow_font");

        settings.setValue("family", font.family());
        settings.setValue("point_size", font.pointSize());

        settings.endGroup();
    }

    QFont getFont()
    {
        settings.beginGroup("mainwindow_font");

        auto family    = settings.value("family", getDefaultFont().family()).toString();
        auto pointSize = settings.value("point_size", getDefaultFont().pointSize()).toInt();

        settings.endGroup();

        return QFont(family, pointSize);
    }

    QFont getDefaultFont()
    {
#ifdef TARGET_OS_UNIX
        return QFont("Helvetica", 18);
#elif _WIN32 || _WIN64
        return QFont("Arial", 18);
#endif
    }

    void saveExceptionsColor(const QColor &c)
    {
        settings.beginGroup("text_color");

        settings.setValue("exceptions_color", c.name());

        settings.endGroup();
    }

    QColor getExceptionsColor()
    {
        settings.beginGroup("text_color");

        auto colorName = settings.value("exceptions_color", getDefaultExceptionsColor()).toString();

        settings.endGroup();

        return QColor(colorName);
    }

    void saveIncorrectColor(const QColor &c)
    {
        settings.beginGroup("text_color");

        settings.setValue("incorrect_color", c.name());

        settings.endGroup();
    }

    QColor getIncorrectColor()
    {
        settings.beginGroup("text_color");

        auto colorName = settings.value("incorrect_color", getDefaultIncorrectColor()).toString();

        settings.endGroup();

        return QColor(colorName);
    }

    QColor getDefaultExceptionsColor() const
    {
        return QColor(60, 179, 113);
    }

    QColor getDefaultIncorrectColor()  const
    {
        return QColor(255,0,0);
    }

    // ==================================================================================================================
    void saveCurrentLanguage(const QString &sourceLanguage, const QString &targetLanguage)
    {
        settings.beginGroup("current_language");

        settings.remove("language1");   // temporary
        settings.remove("language2");   // temporary

        settings.setValue("source_language", sourceLanguage);
        settings.setValue("target_language", targetLanguage);

        settings.endGroup();
    }

    std::tuple<QString, QString> loadCurrentLanguage()
    {
        settings.beginGroup("current_language");

        auto sourceLanguage = settings.value("source_language").toString();
        auto targetLanguage = settings.value("target_language").toString();

        settings.endGroup();

        return std::make_tuple(sourceLanguage, targetLanguage);
    }

    // FOR DEVELOPMENT ONLY =============================================================================================
    /* QString: language — is a concatication of source language name and target language name joined by an underscore */
    void saveLanguageSourceFile(const QString &language, const QString &filename)
    {
        settings.beginGroup("languages");

        settings.setValue(language.toLower() + "/filename", filename);

        settings.endGroup();
    }

    QString loadLanguageSourceFile(const QString &languageId)
    {
        settings.beginGroup("languages");

        auto filename = settings.value(languageId + "/filename", { ":/" + languageId + ".xml" }).toString();

        settings.endGroup();

        return filename;
    }
    // ==================================================================================================================

    /* QString: language — is a concatication of source language name and target language name joined by an underscore */
    void saveLanguageActiveStatus(const QString &languageId, bool isActive)
    {
        settings.beginGroup("languages");

        settings.setValue(languageId + "/isActive", isActive);

        settings.endGroup();
    }

    bool loadLanguageActiveStatus(const QString &languageId)
    {
        settings.beginGroup("languages");

        bool isActive = settings.value(languageId + "/isActive", true).toBool();

        settings.endGroup();

        return isActive;
    }

    void clearLanguages()
    {
        settings.beginGroup("languages");

        settings.clear();

        settings.endGroup();
    }

private:
    QSettings settings;
};

#endif // SETTINGS_H
