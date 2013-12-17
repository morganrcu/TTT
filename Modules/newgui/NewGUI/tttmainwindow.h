#ifndef TTTMAINWINDOW_H
#define TTTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class TTTMainWindow;
}

class TTTMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit TTTMainWindow(QWidget *parent = 0);
    ~TTTMainWindow();
    
private:
    Ui::TTTMainWindow *ui;
};

#endif // TTTMAINWINDOW_H
