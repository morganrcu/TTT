#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include "mysqltissuetrackingproject.h"
#include "tissuetracker.h"
namespace Ui {
    class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();
    inline TissueTracker::ProjectType GetProject(){
        return m_NewProject;
    }
    inline bool IsAccepted(){
    	return m_Accepted;
    }
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_BrowseProject_clicked();
    void on_BrowseImage_clicked();

    void on_NewProjectDialog_accepted();

private:
    Ui::NewProjectDialog *ui;
    bool m_Accepted;
    TissueTracker::ProjectType m_NewProject;
};

#endif // NEWPROJECTDIALOG_H

