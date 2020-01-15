#include "mainwindow.h"
#include <QApplication>
#include <QLocale>

#define DEFLANG "uk"

QString getLocLang()
{
    QString lang;

    lang = QLocale().name().section("_",0,0);

    return lang;
}

void initLang(QApplication *a,QTranslator *translator)
{
    QString lang = getLocLang();

    if(!lang.isEmpty()){
        translator->load(":/translate/asreportdemo_"+ lang +".qm", ".");
        a->installTranslator(translator);
    }

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;

    initLang(&a,&translator);

    MainWindow w;
    w.show();

    //Q_INIT_RESOURCE(images);

    return a.exec();
}
