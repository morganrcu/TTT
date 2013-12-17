#include "tttprojectform.h"
#include "ui_tttprojectform.h"

TTTProjectForm::TTTProjectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TTTProjectForm)
{
    ui->setupUi(this);
}

TTTProjectForm::~TTTProjectForm()
{
    delete ui;
}
