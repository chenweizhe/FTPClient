#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QTextCodec>
//应用程序入口程序
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Set Encode
     QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
     QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));
     QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
    MainWindow w;
    w.show();

    return a.exec();
}
/**
 * Qt是诺基亚公司的一款开源GUI软件框架，本身是基于C++的。
 * 运行效率非常的高，不仅可以用于手机应用程序开发，
 * 同时可以用来开放桌面应用程序。集成了众多GUI类如QMainWindow,QWidget, QDialog等，
 * 同时也重新定义了多个STL类，如QList，QVector， QHash, QMap等。使用起来非常的方便
 *
 */
