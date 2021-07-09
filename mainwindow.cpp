#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initLanguagesAndTransliterators();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLanguagesAndTransliterators()
{
    auto pinyin_ukrainian = LanguagePair::createLanguagePair("file.txt");
    auto german_ukrainian = LanguagePair::createLanguagePair("file.txt");

    languages.add(pinyin_ukrainian);
    languages.add(german_ukrainian);

    transliterators.add(new Transliterator(pinyin_ukrainian));
    transliterators.add(new Transliterator(german_ukrainian));
}

void MainWindow::updateTransliterator(const QString &sourceLanguage, const QString &targetLanguage)
{
    transliterator = transliterators.getTransliterator(sourceLanguage, targetLanguage);
}

void MainWindow::on_textEdit_source_textChanged()
{
    auto words = transliterator->transliterate(ui->textEdit_source->toPlainText());

    WordPainter::paintWords(words, "green", "red");

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
