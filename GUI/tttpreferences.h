#ifndef TTTPREFERENCES_H
#define TTTPREFERENCES_H

#include <QDialog>

namespace Ui {
class TTTPreferences;
}

class TTTPreferences : public QDialog
{
    Q_OBJECT
    
public:
    explicit TTTPreferences(QWidget *parent = 0);
    ~TTTPreferences();
private slots:
    void testConnection();
private:
    Ui::TTTPreferences *m_pUI;

public:
    QString GetHost();
    unsigned int GetPort();
    QString GetDBName();
    QString GetUser();
    QString GetPassword();
    void SetHost(const QString & host);
    void SetPort(unsigned int port);
    void SetDBName(const QString & dbName);
    void SetUser(const QString & user);
    void SetPassword(const QString & password);
private:

};

#endif // TTTPREFERENCES_H
