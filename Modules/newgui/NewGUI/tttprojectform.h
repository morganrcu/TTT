#ifndef TTTPROJECTFORM_H
#define TTTPROJECTFORM_H

#include <QWidget>

namespace Ui {
class TTTProjectForm;
}

class TTTProjectForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit TTTProjectForm(QWidget *parent = 0);
    ~TTTProjectForm();
    
private:
    Ui::TTTProjectForm *ui;
};

#endif // TTTPROJECTFORM_H
