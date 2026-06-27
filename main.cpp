#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qscreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Menu_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
    w.showMaximized();
    return a.exec();
}
