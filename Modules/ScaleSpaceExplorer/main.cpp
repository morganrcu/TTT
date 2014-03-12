#include "scalespaceexplorer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScaleSpaceExplorer w;
    w.show();

    return a.exec();
}
