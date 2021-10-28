QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    preferencesdialog.cpp \
    preferencesdialogdev.cpp

HEADERS += \
    LanguageStructures/LanguagePair.h \
    LanguageStructures/LanguagePairList.h \
    LanguageStructures/SyllablePair.h \
    LanguageStructures/SyllablePairSet.h \
    Parsers/Words.h \
    Settings/Settings.h \
    SyllableTree/SyllableTree.h\
    Parsers/XMLParser.h \
    Transliterators/GermanUkrainianTransliterator.h \
    Transliterators/PassportUkrainianTransliterator.h \
    Transliterators/PinyinUkrainianTransliterator.h \
    Transliterators/ScientificUkrainianTransliterator.h \
    Transliterators/SpanishUkrainianTransliterator.h \
    Transliterators/Transliterator.h \
    Transliterators/TransliteratorList.h \
    mainwindow.h \
    preferencesdialog.h \
    preferencesdialogdev.h

FORMS += \
    mainwindow.ui \
    preferencesdialog.ui \
    preferencesdialogdev.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    libraries.qrc

maxc {
    ICON = Logo.icns
}
