#ifndef GCMDMAINWINDOW_H
#define GCMDMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class GCMDMainWindow;
}

class GCMDMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GCMDMainWindow(QWidget *parent = 0);
    ~GCMDMainWindow();

private slots:
    void changeDrive(int index);

private:
    Ui::GCMDMainWindow *ui;
};

#endif // GCMDMAINWINDOW_H
