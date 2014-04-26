#include "tttmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TTTMainWindow w;
    w.show();
    
    return a.exec();
}

