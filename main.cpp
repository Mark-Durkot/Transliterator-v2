#include "mainwindow.h"

#include "LanguageStructures/SyllablePair.h"
#include "SyllableTree/SyllableTree.h"

#include <QApplication>
#include <QtXml>
#include <QString>
#include <QStringList>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
