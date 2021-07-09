#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Transliterators/TransliteratorList.h"
#include "LanguageStructures/LanguagePairList.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initLanguagesAndTransliterators();

    void setUpComboBoxes();

    void updateTransliterator(const QString &sourceLanguage, const QString &targetLanguage);

private slots:
    void on_textEdit_source_textChanged();

    void on_comboBox_source_activated(const QString &arg1);

    void on_comboBox_target_activated(const QString &arg1);

    void on_pushButton_swap_clicked();

private:
    Ui::MainWindow *ui;

    LanguagePairList   languages;
    TransliteratorList transliterators;
    Transliterator     *transliterator;
};
#endif // MAINWINDOW_H
