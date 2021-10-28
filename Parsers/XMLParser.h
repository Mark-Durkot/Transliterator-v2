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
                auto source  = element.attribute("source").toUtf8();
                auto target  = element.attribute("target").toUtf8();
                auto type    = element.attribute("type"  ).toInt();

                set.insert(SyllablePair(source, target, static_cast<SyllableType>(type)));
            }
            else
            {
                throw QString("Item " + items.at(i).nodeName() + " is not an element");
            }
        }

        return set;
    }

    QString parseFirstLanguageName()  { return root.attribute("source_language");       }

    QString parseSecondLanguageName() { return root.attribute("target_language");       }

    bool parseTwoWayTransliteration() { return root.attribute("bidirectional").toInt(); }

private:
    QDomDocument document;
    QDomElement root;
    QFile file;
};

#endif // XMLPARSER_H
