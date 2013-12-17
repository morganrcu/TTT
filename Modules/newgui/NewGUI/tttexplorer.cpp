#include "tttexplorer.h"
#include "ui_tttexplorer.h"

tttExplorer::tttExplorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tttExplorer)
{
    ui->setupUi(this);
}

tttExplorer::~tttExplorer()
{
    delete ui;
}
