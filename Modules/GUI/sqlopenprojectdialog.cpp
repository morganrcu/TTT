#include "sqlopenprojectdialog.h"
#include "ui_sqlopenprojectdialog.h"
#include <QtSql/qsqldatabase.h>
#include <iostream>
#include <QtSql/qsqlrelationaltablemodel.h>
SQLOpenProjectDialog::SQLOpenProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SQLOpenProjectDialog)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("TuftsTissueTracker");
    db.setUserName("root");
    db.setPassword("tracker");

    bool ok=db.open();
    assert(ok);
    if(ok){
    	QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this,db);
    	model->setTable("Project");
    	model->setEditStrategy(QSqlTableModel::OnRowChange);
    	model->select();
    	ui->existingProjectsTableView->setModel(model);
    }
    m_Accepted=false;

}

void SQLOpenProjectDialog::accept(){
	int projectID=atoi(ui->existingProjectsTableView->model()->data(ui->existingProjectsTableView->currentIndex()).toString().toStdString().c_str());

	m_Project=new TissueTrackingProject;
	m_Project->openDB();
	m_Project->OpenProject(projectID);
	m_Accepted=true;

	QDialog::accept();
}
SQLOpenProjectDialog::~SQLOpenProjectDialog()
{
    delete ui;
}
