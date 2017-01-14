#include "mainwindow.h"
#include "connectform.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.move ((QApplication::desktop()->width() - w.width())/2, (QApplication::desktop()->height() - w.height())/2); // 将窗口移动到屏幕中央(http://blog.csdn.net/liang890319/article/details/6997733)
    w.show();

    return a.exec();
}
