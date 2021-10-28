#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "Transliterators/TransliteratorList.h"
#include "LanguageStructures/LanguagePairList.h"

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = nullptr);
    ~PreferencesDialog();

    void setLanguages();
    void setTransliterators();

private:
    void setupLanguagesLayout();

private:
    Ui::PreferencesDialog *ui;

    LanguagePairList languages;
    TransliteratorList transliterators;
};

#endif // PREFERENCESDIALOG_H
