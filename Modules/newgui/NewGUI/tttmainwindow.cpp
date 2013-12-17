#include "tttmainwindow.h"
#include "ui_tttmainwindow.h"

TTTMainWindow::TTTMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TTTMainWindow)
{
    ui->setupUi(this);
}

TTTMainWindow::~TTTMainWindow()
{
    delete ui;
}
