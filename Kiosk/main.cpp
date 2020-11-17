#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setFixedSize(1200,742);
    w.setWindowFlags( Qt::CustomizeWindowHint );
    //w.showFullScreen(); // 전체화면 모드
    //w.showMaximized(); // 최대화
    //w.setWindowFlags( Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    w.show();
    return a.exec();
}
