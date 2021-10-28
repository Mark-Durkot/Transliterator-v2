#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Transliterators/TransliteratorList.h"
#include "Transliterators/PinyinUkrainianTransliterator.h"
#include "Transliterators/GermanUkrainianTransliterator.h"
#include "Transliterators/SpanishUkrainianTransliterator.h"
#include "Transliterators/PassportUkrainianTransliterator.h"
#include "Transliterators/ScientificUkrainianTransliterator.h"

#include "LanguageStructures/LanguagePairList.h"
#include "Settings/Settings.h"

#include "preferencesdialog.h"
#include "preferencesdialogdev.h"

#include <chrono>

#include <QStandardItem>
#include <QStandardItemModel>
#include <QMainWindow>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std::chrono;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initLanguagesAndTransliterators();

    void setupComboBoxes();

    void setupTextEdits();

    void updateCurrentTransliterator(const QString &sourceLanguage, const QString &targetLanguage);

    void updateSwapButtonStatus();

    void activateTargetComboBoxItems();

    void runDevelopersPreferences();

    void runUsersPreferences();

private slots:
    void on_textEdit_source_textChanged();

    void on_pushButton_swap_clicked();

//    void on_comboBox_source_textActivated(const QString &arg1);

//    void on_comboBox_target_textActivated(const QString &arg1);

    void on_actionPreferences_triggered();

    void on_comboBox_source_currentTextChanged(const QString &arg1);

    void on_comboBox_target_currentTextChanged(const QString &arg1);

    void on_actionUpdate_Libraries_triggered();

private:
    void writeSettings();
    void readSettings();

private:
    Ui::MainWindow *ui;

    LanguagePairList   languages;
    TransliteratorList transliterators;
    Transliterator     *currentTransliterator;

    WordPainter painter;

    Settings settings { "mark-durkot", "Transliterator" };

};
#endif // MAINWINDOW_H
