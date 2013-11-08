/*
 * sqlnewprojectdialog.h
 *
 *  Created on: Oct 24, 2013
 *      Author: morgan
 */

#ifndef SQLNEWPROJECTDIALOG_H_
#define SQLNEWPROJECTDIALOG_H_


#include <QDialog>
#include <mysqltissuetrackingproject.h>

namespace Ui {
    class SQLNewProjectDialog;
}
using namespace ttt;
class SQLNewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SQLNewProjectDialog(QWidget *parent = 0);
    ~SQLNewProjectDialog();
    TissueTrackingProject * GetNewProject(){
    	return m_NewProject;

    }
    inline bool IsAccepted(){
    	return m_Accepted;
    }
private slots:
    void addFiles();
    void selectWorkingDirectory();
private:
    void addFileToList();
    void accept();

    Ui::SQLNewProjectDialog *ui;

    bool m_Accepted;

    TissueTrackingProject * m_NewProject;
};


#endif /* SQLNEWPROJECTDIALOG_H_ */
