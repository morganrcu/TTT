#ifndef TTTEXPLORER_H
#define TTTEXPLORER_H

#include <QDialog>

namespace Ui {
class tttExplorer;
}

class tttExplorer : public QDialog
{
    Q_OBJECT
    
public:
    explicit tttExplorer(QWidget *parent = 0);
    ~tttExplorer();
    
private:
    Ui::tttExplorer *ui;
};

#endif // TTTEXPLORER_H
