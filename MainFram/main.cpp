#include <QApplication>
#include <QTextCodec>
#include <QDesktopWidget>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QFont f("ZYSong18030",12);//
    a.setFont(f);


    MainWindow w;
    w.resize(800, 700);
    w.show();

    w.move((QApplication::desktop()->width() - w.width())/2,
          (QApplication::desktop()->height() - w.height())/2);

    return a.exec();
}
