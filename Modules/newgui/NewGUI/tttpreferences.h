#ifndef TTTPREFERENCES_H
#define TTTPREFERENCES_H

#include <QDialog>

namespace Ui {
class tttPreferences;
}

class tttPreferences : public QDialog
{
    Q_OBJECT
    
public:
    explicit tttPreferences(QWidget *parent = 0);
    ~tttPreferences();
private slots:
    void testConnection();
private:
    Ui::tttPreferences *ui;
};

#endif // TTTPREFERENCES_H
