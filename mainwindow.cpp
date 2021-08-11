#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initLanguagesAndTransliterators();

    // temporary ----------------------------------
    painter.setExceptionColor(QColor(0,255,0));
    painter.setIncorrectColor(QColor(255,0,0));
    // temporary ----------------------------------

    setupTextEdits();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLanguagesAndTransliterators()
{
    auto pinyin_ukrainian     = LanguagePair::createLanguagePair("/Users/md/Desktop/pinyin_ukrainian.xml");
    auto german_ukrainian     = LanguagePair::createLanguagePair("/Users/md/Desktop/german_ukrainian.xml");
    //auto passport_ukrainian   = LanguagePair::createLanguagePair("/Users/md/Desktop/passport_ukrainian.xml");
    //auto scientific_ukrainian = LanguagePair::createLanguagePair("/Users/md/Desktop/scientific_ukrainian.xml");

    languages.add(pinyin_ukrainian);
    languages.add(german_ukrainian);
    //languages.add(passport_ukrainian);
    //languages.add(scientific_ukrainian);

    //transliterators.add(new PinyinUkrainianTransliterator(pinyin_ukrainian));
    transliterators.add(new PinyinUkrainianTransliterator(pinyin_ukrainian));
    transliterators.add(new Transliterator(german_ukrainian));
    //transliterators.add(new Transliterator(passport_ukrainian));
    //transliterators.add(new Transliterator(scientific_ukrainian));

    currentTransliterator = transliterators.first();
}

void MainWindow::setupTextEdits()
{
    ui->textEdit_source->setAcceptRichText(false);
    ui->textEdit_source->setFocus();
}

void MainWindow::updateTransliterator(const QString &sourceLanguage, const QString &targetLanguage)
{
    currentTransliterator = transliterators.getTransliterator(sourceLanguage, targetLanguage);
}

void MainWindow::on_textEdit_source_textChanged()
{
    auto words = currentTransliterator->transliterate(ui->textEdit_source->toPlainText());

    painter.paintWords(words);

    ui->textEdit_target->setHtml(words.toHtmlText());
}

void MainWindow::on_comboBox_source_activated(const QString &arg1)
{
    auto sourceLanguage = arg1;
    auto targetLanguage = ui->comboBox_target->currentText();

    updateTransliterator(sourceLanguage, targetLanguage);
}

void MainWindow::on_comboBox_target_activated(const QString &arg1)
{
    auto sourceLanguage = ui->comboBox_source->currentText();
    auto targetLanguage = arg1;

    updateTransliterator(sourceLanguage, targetLanguage);
}

void MainWindow::on_pushButton_swap_clicked()
{
    auto sourceLanguage = ui->comboBox_source->currentText();
    auto targetLanguage = ui->comboBox_target->currentText();

    qSwap(sourceLanguage, targetLanguage);

    ui->comboBox_source->setCurrentText(sourceLanguage);
    ui->comboBox_target->setCurrentText(targetLanguage);

    updateTransliterator(sourceLanguage, targetLanguage);
}
