#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "../LanguageStructures/SyllablePairSet.h"

#include <QtXml>
#include <QFile>

class XMLParser
{
public:
    void setContent(const QString &filename)
    {
        file.setFileName(filename);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            if (document.setContent(&file))
            {
                root = document.firstChildElement();
            }
        }

        file.close();
    }

    SyllablePairSet parseSetByTagName(const QString tagname)
    {
        auto items = root.elementsByTagName(tagname);

        SyllablePairSet set;

        for (int i = 0; i < items.count(); ++i)
        {
            if (items.at(i).isElement())
            {
                auto element = items.at(i).toElement();
                set.insert(SyllablePair(element.attribute("first").toUtf8(), element.attribute("second").toUtf8()));
            }
            else
            {
                throw "Item " + items.at(i).nodeName() + " is not an element";
            }
        }

        return set;
    }

    QString parseFirstLanguageName()  { return root.attribute("first_language");  }

    QString parseSecondLanguageName() { return root.attribute("second_language"); }

    bool parseTwoWayTransliteration() { return root.attribute("twt").toInt();     }

private:
    QDomDocument document;
    QDomElement root;
    QFile file;
};

#endif // XMLPARSER_H
