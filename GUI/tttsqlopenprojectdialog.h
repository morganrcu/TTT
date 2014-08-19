/** \addtogroup TTTGUI
 *  @{
 */
#ifndef SQLOPENPROJECTDIALOG_H
#define SQLOPENPROJECTDIALOG_H

#include <QDialog>
#include "mysqltissuetrackingproject2.h"
namespace Ui {

class SQLOpenProjectDialog;
}
using namespace ttt;
/**
 * \class SQLOpenProjectialog
 * TODO
 */
class SQLOpenProjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * TODO
     * @param parent
     */
    explicit SQLOpenProjectDialog(const QString & host, unsigned int port, const QString & dbname, const QString & user, const QString & password,QWidget *parent = 0);
    ~SQLOpenProjectDialog();

    /**
     * TODO
     * @return
     */
    inline TissueTrackingAbstractProject2 * GetProject(){
    	return m_Project;
    }
    /**
     * TODO
     * @return
     */
    inline bool IsAccepted(){
    	return m_Accepted;
    }
private slots:
/**
 *
 */
	void accept();
    
private:
	/**
	 *
	 */
    Ui::SQLOpenProjectDialog *ui;

    /**
     *
     */
    MySQLTissueTrackingProject2 * m_Project;

    /**
     *
     */
    bool m_Accepted;

    QString m_Host;
    unsigned int m_Port;
    QString m_DBName;
    QString m_User;
    QString m_Password;
};

#endif // SQLOPENPROJECTDIALOG_H
/** @}*/
