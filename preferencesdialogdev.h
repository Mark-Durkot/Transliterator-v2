#ifndef PREFERENCESDIALOGDEV_H
#define PREFERENCESDIALOGDEV_H

#include "LanguageStructures/LanguagePair.h"
#include "Transliterators/TransliteratorList.h"
#include "LanguageStructures/LanguagePairList.h"
#include "Settings/Settings.h"

#include <QDialog>
#include <QFile>
#include <QMap>
#include <QColor>

#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>

namespace Ui {
class PreferencesDialogDev;
}

class PreferencesDialogDev : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialogDev(TransliteratorList *transliterators,
                                  LanguagePairList *languages,
                                  QWidget *parent = nullptr);
    ~PreferencesDialogDev();

private slots:
    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_pushButton_browse_clicked();

    void on_buttonBox_accepted();

    void on_pushButton_exceptions_color_clicked();

    void on_pushButton_incorrect_color_clicked();

    void on_buttonBox_rejected();

    void on_pushButton_default_exception_color_clicked();

    void on_pushButton_default_incorrect_color_clicked();

    void on_pushButton_use_built_in_clicked();

private:
    void setupLanguageMap();

    void setupColorButtons();

    void setupFont();

    bool updateLanguage(LanguagePair *language);

    void writeSettings();

    void writeLanguageSourceFileMap();

    void writeColorsSettings();

    void writeFontSettings();

    void writeTranslationSettings();

    void translateUi();

    QString getStyleSheetForColorPushButton(const QColor &c);

private:
    Ui::PreferencesDialogDev *ui;

    /* key - language id, value - source file */
    QMap<QString, QString> languageSourceFileMap;

    TransliteratorList *transliterators;
    LanguagePairList   *languages;

    Settings settings { "mark-durkot", "Transliterator" };

    QColor exceptionsColor;
    QColor  incorrectColor;
};

#endif // PREFERENCESDIALOGDEV_H
