#include "tttpreferences.h"
#include "ui_tttpreferences.h"

tttPreferences::tttPreferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tttPreferences)
{
    ui->setupUi(this);
}

tttPreferences::~tttPreferences()
{
    delete ui;
}

void tttPreferences::testConnection(){

}
