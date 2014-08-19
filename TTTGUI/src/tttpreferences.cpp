#include <QtSql/qsqldatabase.h>
#include <QMessageBox>
#include "tttpreferences.h"
#include "ui_tttpreferences.h"

TTTPreferences::TTTPreferences(QWidget *parent) :
    QDialog(parent),
    m_pUI(new Ui::TTTPreferences)
{
	m_pUI->setupUi(this);
	connect(this->m_pUI->testButton,SIGNAL(clicked()),this,SLOT(testConnection()));
}

TTTPreferences::~TTTPreferences()
{
    delete m_pUI;
}

void TTTPreferences::testConnection(){
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(this->m_pUI->hostTxt->text());
	db.setDatabaseName(this->m_pUI->DBNameTxt->text());
	db.setUserName(this->m_pUI->userTxt->text());
	db.setPassword(this->m_pUI->passwordTxt->text());
	db.setPort(this->m_pUI->portSpinBox->value());
	bool ok=db.open();

	QMessageBox msgBox;

	if(ok){
		msgBox.setText("Succesfully connected to the DB");
	}else{
		msgBox.setText("Could not connect to the DB. Check the specified parameters");
	}
	msgBox.exec();
}

QString TTTPreferences::GetHost(){
  	return this->m_pUI->hostTxt->text();
  }
  unsigned int TTTPreferences::GetPort(){
  	return (unsigned int)( this->m_pUI->portSpinBox->value());
  }
QString TTTPreferences::GetDBName(){
  	return this->m_pUI->DBNameTxt->text();
}
QString TTTPreferences::GetUser(){
	return this->m_pUI->userTxt->text();
}
QString TTTPreferences::GetPassword(){
  	return this->m_pUI->passwordTxt->text();
}
void TTTPreferences::SetHost(const QString & host){
	this->m_pUI->hostTxt->setText(host);
}
void TTTPreferences::SetPort(unsigned int port){
	this->m_pUI->portSpinBox->setValue(port);
}
void TTTPreferences::SetDBName(const QString & dbName){
	this->m_pUI->DBNameTxt->setText(dbName);
}
void TTTPreferences::SetUser(const QString & user){
  	this->m_pUI->userTxt->setText(user);
}
void TTTPreferences::SetPassword(const QString & password){
  	this->m_pUI->passwordTxt->setText(password);
}
