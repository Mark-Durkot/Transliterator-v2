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

    setupScrollBars();
    setupComboBoxes();
    setupTextEdits();

    readSettings();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::initLanguagesAndTransliterators()
{   
    auto pinyin_ukrainian     = LanguagePair::createLanguagePair(settings.loadLanguageSourceFile("pinyin_ukrainian"));
    auto german_ukrainian     = LanguagePair::createLanguagePair(settings.loadLanguageSourceFile("german_ukrainian"));
    auto spanish_ukrainian    = LanguagePair::createLanguagePair(settings.loadLanguageSourceFile("spanish_ukrainian"));
    auto ukrainian_passport   = LanguagePair::createLanguagePair(settings.loadLanguageSourceFile("ukrainian_passport"));
    auto ukrainian_scientific = LanguagePair::createLanguagePair(settings.loadLanguageSourceFile("ukrainian_scientific"));

//    auto pinyin_ukrainian     = LanguagePair::createLanguagePair(":/pinyin_ukrainian.xml");
//    auto german_ukrainian     = LanguagePair::createLanguagePair(":/german_ukrainian.xml");
//    auto spanish_ukrainian    = LanguagePair::createLanguagePair(":/spanish_ukrainian.xml");
//    auto ukrainian_passport   = LanguagePair::createLanguagePair(":/ukrainian_passport.xml");
//    auto ukrainian_scientific = LanguagePair::createLanguagePair(":/ukrainian_scientific.xml");

    languages.add(pinyin_ukrainian);
    languages.add(german_ukrainian);
    languages.add(spanish_ukrainian);
    languages.add(ukrainian_passport);
    languages.add(ukrainian_scientific);

    transliterators.add(new PinyinUkrainianTransliterator    (pinyin_ukrainian));
    transliterators.add(new GermanUkrainianTransliterator    (german_ukrainian));
    transliterators.add(new SpanishUkrainianTransliterator   (spanish_ukrainian));
    transliterators.add(new PassportUkrainianTransliterator  (ukrainian_passport));
    transliterators.add(new ScientificUkrainianTransliterator(ukrainian_scientific));
}

void MainWindow::setupComboBoxes()
{
    ui->comboBox_source->clear();
    ui->comboBox_target->clear();

    ui->comboBox_source->addItems(languages.getSourceLanguages());
    ui->comboBox_target->addItems(languages.getAllTargetLanguages());

    activateTargetComboBoxItems();
}

void MainWindow::setupTextEdits()
{
    ui->textEdit_source->setAcceptRichText(false);
    ui->textEdit_source->setFocus();

    ui->textEdit_target->setAcceptRichText(true);
}

void MainWindow::setupScrollBars()
{
    sourceScrollBar = new QScrollBar(ui->textEdit_source->horizontalScrollBar());
    targetScrollBar = new QScrollBar(ui->textEdit_target->horizontalScrollBar());

    connect(sourceScrollBar, &QScrollBar::valueChanged, targetScrollBar, &QScrollBar::setValue);
    connect(targetScrollBar, &QScrollBar::valueChanged, sourceScrollBar, &QScrollBar::setValue);
}

void MainWindow::updateCurrentTransliterator(const QString &sourceLanguage, const QString &targetLanguage)
{
    auto newTransliterator = transliterators.getTransliterator(sourceLanguage, targetLanguage);

    if (newTransliterator)
    {
        currentTransliterator = newTransliterator;
    }
    else
    {
        newTransliterator = transliterators.getDefaultForSourceLanguage(sourceLanguage);

        if (newTransliterator)
        {
            currentTransliterator = newTransliterator;
            ui->comboBox_source->setCurrentText(currentTransliterator->getCurrentSourceLanguage());
            ui->comboBox_target->setCurrentText(currentTransliterator->getCurrentTargetLanguage());
        }
    }
}

void MainWindow::updateSwapButtonStatus()
{
    if (currentTransliterator->isTwoWayTransliteration())
    {
        ui->pushButton_swap->setEnabled(true);
    }
    else
    {
        ui->pushButton_swap->setEnabled(false);
    }
}

void MainWindow::activateTargetComboBoxItems()
{
    auto source = ui->comboBox_source->currentText();

    auto targetLanguages = languages.getAvailableTargetLanguages(source);

    auto model = dynamic_cast<QStandardItemModel*>(ui->comboBox_target->model());

    for (int i = 0; i < model->rowCount(); ++i)
    {
        if (targetLanguages.contains(model->item(i)->text()))
        {
            model->item(i)->setEnabled(true);
        }
        else
        {
            model->item(i)->setEnabled(false);
        }
    }
}

void MainWindow::runDevelopersPreferences()
{
    PreferencesDialogDev dialog(&transliterators, &languages, this);
    dialog.exec();

    painter.setExceptionColor(settings.getExceptionsColor());
    painter.setIncorrectColor(settings.getIncorrectColor());

    ui->textEdit_source->setFont(settings.getFont());
    ui->textEdit_target->setFont(settings.getFont());
}

void MainWindow::on_textEdit_source_textChanged()
{   
    QElapsedTimer timer;

    timer.restart();
    auto words = currentTransliterator->transliterate(ui->textEdit_source->toPlainText());
    auto time = timer.elapsed();
    qDebug() << time;

    painter.paintWords(words);

    ui->textEdit_target->setHtml(words.toHtmlText());
}

void MainWindow::on_pushButton_swap_clicked()
{
    auto sourceLanguage = ui->comboBox_source->currentText();
    auto targetLanguage = ui->comboBox_target->currentText();

    qSwap(sourceLanguage, targetLanguage);

    updateCurrentTransliterator(sourceLanguage, targetLanguage);

    activateTargetComboBoxItems();

    ui->comboBox_source->setCurrentText(sourceLanguage);
    ui->comboBox_target->setCurrentText(targetLanguage);

    ui->textEdit_source->setText(ui->textEdit_target->toPlainText());
}

void MainWindow::writeSettings()
{
    settings.saveMainWindowGeometry(this->geometry());

    settings.saveFont(ui->textEdit_source->font());

    settings.saveCurrentLanguage(currentTransliterator->getCurrentSourceLanguage(),
                                 currentTransliterator->getCurrentTargetLanguage());

    for (auto t : transliterators)
    {
        settings.saveLanguageActiveStatus(t->getSourceLanguageName() + "_" + t->getTargetLanguageName(), true);
    }
}

void MainWindow::readSettings()
{
    painter.setExceptionColor(settings.getExceptionsColor());
    painter.setIncorrectColor(settings.getIncorrectColor());

    QString sourceLanguage, targetLanguage;
    std::tie(sourceLanguage, targetLanguage) = settings.loadCurrentLanguage();

    ui->comboBox_source->setCurrentText(sourceLanguage);
    ui->comboBox_target->setCurrentText(targetLanguage);

    auto font     = settings.getFont();
    auto geometry = settings.loadMainWindowGeometry();

    ui->textEdit_source->setFont(font);
    ui->textEdit_target->setFont(font);

    this->setGeometry(geometry);
}

void MainWindow::on_actionPreferences_triggered()
{
    runDevelopersPreferences();
}

void MainWindow::on_comboBox_source_currentTextChanged(const QString &arg1)
{
    auto sourceLanguage = arg1;
    auto targetLanguage = ui->comboBox_target->currentText();

    updateCurrentTransliterator(sourceLanguage, targetLanguage);
    activateTargetComboBoxItems();
    updateSwapButtonStatus();
}

void MainWindow::on_comboBox_target_currentTextChanged(const QString &arg1)
{
    auto sourceLanguage = ui->comboBox_source->currentText();
    auto targetLanguage = arg1;

    updateCurrentTransliterator(sourceLanguage, targetLanguage);
    updateSwapButtonStatus();
}

void MainWindow::on_actionUpdate_Libraries_triggered()
{
    languages.clearData();
    transliterators.clearData();

    initLanguagesAndTransliterators();

    auto currentSource = ui->comboBox_source->currentText();
    auto currentTarget = ui->comboBox_target->currentText();

    setupComboBoxes();

    ui->comboBox_source->setCurrentText(currentSource);
    ui->comboBox_target->setCurrentText(currentTarget);
}

void MainWindow::on_actionClear_triggered()
{
    ui->textEdit_source->clear();
    ui->textEdit_target->clear();
}
