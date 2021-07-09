#ifndef SETTINGS_H
#define SETTINGS_H

#include <QRect>
#include <QFont>

class Settings
{
public:
    void saveMainWindowGeometry(const QRect &geometry)
    {

    }

    QRect loadMainWindowGeometry() const
    {
        return {};
    }

    void saveFont(const QFont &font)
    {

    }

    QFont loadFont() const
    {

    }

private:

};

#endif // SETTINGS_H
