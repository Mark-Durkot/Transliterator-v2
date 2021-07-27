#ifndef SETTINGS_H
#define SETTINGS_H

#include <QRect>
#include <QFont>
#include <QSettings>
#include <QGuiApplication>
#include <QScreen>

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

        settings.setValue("position",  geometry.topLeft());
        settings.setValue("size", geometry.size());

        settings.endGroup();
    }

    QRect loadMainWindowGeometry()
    {
        settings.beginGroup("mainwindow_geometry");

        auto pos  = settings.value("position", QGuiApplication::screens().at(0)->geometry().center()).toPoint();
        auto size = settings.value("size", QSize(300, 200)).toSize(); // temporary default size

        settings.endGroup();

        return QRect(pos, size);
    }

    void saveFont(const QFont &font)
    {
        settings.beginGroup("mainwindow_font");

        settings.setValue("family", font.family());
        settings.setValue("point_size", font.pointSize());

        settings.endGroup();
    }

    QFont loadFont()
    {
        settings.beginGroup("mainwindow_font");

        auto family    = settings.value("family", "Helvetica").toString();
        auto pointSize = settings.value("point_size", 18).toInt();

        settings.endGroup();

        return QFont(family, pointSize);
    }

    void saveCurrentLanguage(const QString &lang1, const QString &lang2)
    {
        settings.beginGroup("current_language");

        settings.setValue("language1", lang1);
        settings.setValue("language2", lang2);

        settings.endGroup();
    }

    QStringList loadCurrentLanguage()
    {
        settings.beginGroup("current_language");

        auto lang1 = settings.value("language1").toString();
        auto lang2 = settings.value("language2").toString();

        return { lang1, lang2 };
    }

private:
    QSettings settings;
};

#endif // SETTINGS_H
