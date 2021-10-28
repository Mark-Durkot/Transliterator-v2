#include "preferencesdialogdev.h"
#include "ui_preferencesdialogdev.h"

PreferencesDialogDev::PreferencesDialogDev(TransliteratorList *transliterators,
                                           LanguagePairList *languages,
                                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialogDev),
    transliterators(transliterators),
    languages(languages)
{
    ui->setupUi(this);

    setupLanguageMap();
    setupColorButtons();
    setupFont();
}

PreferencesDialogDev::~PreferencesDialogDev()
{
    delete ui;
}

void PreferencesDialogDev::setupLanguageMap()
{
    for (auto l : *languages)
    {
        auto sourceFile = settings.loadLanguageSourceFile(l->getLanguagePairId());
        auto languageId = l->getLanguagePairId();

        languageSourceFileMap.insert(languageId, sourceFile);
        ui->listWidget->addItem(languageId);
    }
}

void PreferencesDialogDev::setupColorButtons()
{
    exceptionsColor = settings.getExceptionsColor();
    incorrectColor  = settings.getIncorrectColor();

    ui->pushButton_exceptions_color->setStyleSheet(getStyleSheetForColorPushButton(exceptionsColor));
    ui->pushButton_incorrect_color->setStyleSheet(getStyleSheetForColorPushButton(incorrectColor));
}

void PreferencesDialogDev::setupFont()
{
    ui->fontComboBox->setCurrentFont(settings.getFont());
    ui->spinBox_font_size->setValue(settings.getFont().pointSize());
}

void PreferencesDialogDev::writeSettings()
{
    writeLanguageSourceFileMap();
    writeTranslationSettings();
    writeColorsSettings();
    writeFontSettings();
}

void PreferencesDialogDev::writeLanguageSourceFileMap()
{
    QMapIterator<QString, QString> iterator(languageSourceFileMap);

    while (iterator.hasNext())
    {
        iterator.next();
        settings.saveLanguageSourceFile(iterator.key(), iterator.value());
    }
}

void PreferencesDialogDev::writeColorsSettings()
{
    ui->pushButton_exceptions_color->palette();
}

void PreferencesDialogDev::writeFontSettings()
{
    auto family   = ui->fontComboBox->currentText();
    auto fontSize = ui->spinBox_font_size->value();

    settings.saveFont({ family, fontSize });
}

void PreferencesDialogDev::writeTranslationSettings()
{
    settings.saveExceptionsColor(exceptionsColor);
    settings.saveIncorrectColor (incorrectColor );
}

QString PreferencesDialogDev::getStyleSheetForColorPushButton(const QColor &c)
{
    QColor hoveredColor = QColor(c.red() / 2, c.green() / 2, c.blue() / 2);
    QColor pressedColor = QColor(fabs(c.red() - 5), fabs(c.green() - 5), fabs(c.blue() - 5));

    QString styleSheet = QString(
                "QPushButton {"
                "background : %2;"
                "border: 1px solid gray;"
                "border-radius: 4px;"
                "}"
                "QPushButton:hover {"
                "background : %3;"
                "}"
                "QPushButton:pressed {"
                "background : %4;"
                "}"
                ).arg(c.name(), hoveredColor.name(), pressedColor.name());

    return styleSheet;
}

void PreferencesDialogDev::on_listWidget_currentTextChanged(const QString &currentText)
{
    ui->lineEdit_sourceFile->setText(languageSourceFileMap.value(currentText));
}

void PreferencesDialogDev::on_pushButton_browse_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);

    auto language = LanguagePair::createLanguagePair(filename);

    if (!language)
    {
        QMessageBox::critical(this, "", "Failed to load file: " + filename);
        return;
    }

    for (auto &l : *languages)
    {
        if (l->getLanguagePairId() == language->getLanguagePairId())
        {
            l = language;

            for (auto t : *transliterators)
            {
                if (t->getLanguagePair()->getLanguagePairId() == language->getLanguagePairId())
                {
                    t->setLanguagePair(language);
                }
            }

            break;
        }
    }

    languageSourceFileMap.insert(language->getLanguagePairId(), filename);
    ui->lineEdit_sourceFile->setText(filename);
}


void PreferencesDialogDev::on_buttonBox_accepted()
{
    writeSettings();
    this->accept();
}


void PreferencesDialogDev::on_pushButton_exceptions_color_clicked()
{
    QColor color = QColorDialog::getColor(exceptionsColor, this);

    if (color.isValid())
    {
        exceptionsColor = color;
        ui->pushButton_exceptions_color->setStyleSheet(getStyleSheetForColorPushButton(exceptionsColor));
    }
}


void PreferencesDialogDev::on_pushButton_incorrect_color_clicked()
{
    QColor color = QColorDialog::getColor(incorrectColor, this);

    if (color.isValid())
    {
        incorrectColor = color;
        ui->pushButton_incorrect_color->setStyleSheet(getStyleSheetForColorPushButton(incorrectColor));
    }
}


void PreferencesDialogDev::on_buttonBox_rejected()
{
    this->reject();
}


void PreferencesDialogDev::on_pushButton_default_exception_color_clicked()
{
    auto defaultColor = settings.getDefaultExceptionsColor();
    settings.saveExceptionsColor(defaultColor);
    ui->pushButton_exceptions_color->setStyleSheet(getStyleSheetForColorPushButton(defaultColor));
}


void PreferencesDialogDev::on_pushButton_default_incorrect_color_clicked()
{
    auto defaultColor = settings.getDefaultIncorrectColor();
    settings.saveIncorrectColor(defaultColor);
    ui->pushButton_incorrect_color->setStyleSheet(getStyleSheetForColorPushButton(defaultColor));
}

