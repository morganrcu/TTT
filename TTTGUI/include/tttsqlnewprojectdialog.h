/** \addtogroup TTTGUI
 *  @{
 */
#ifndef SQLNEWPROJECTDIALOG_H_
#define SQLNEWPROJECTDIALOG_H_


#include <QDialog>
#include "tissuetrackingabstractproject2.h"

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
    TissueTrackingAbstractProject2<3> * GetNewProject(){
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

    TissueTrackingAbstractProject2<3> * m_NewProject;
};


#endif /* SQLNEWPROJECTDIALOG_H_ */
/** @}*/
