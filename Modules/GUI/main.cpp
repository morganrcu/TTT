#include <QApplication>
#include "tissuetracker.h"
//#include "form.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TissueTracker w;
    w.show();


    return a.exec();
}
