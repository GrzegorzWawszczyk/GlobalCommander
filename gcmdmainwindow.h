#ifndef GCMDMAINWINDOW_H
#define GCMDMAINWINDOW_H

#include "filesview.h"
#include "settingswindow.h"

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class GCMDMainWindow;
}

class GCMDMainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit GCMDMainWindow(QWidget *parent = 0);
    ~GCMDMainWindow();


public slots:
    void addTabLeft(const QDir& path);
    void addTabRight(const QDir& path);

//private slots:
//    void changeLeftDrive(int index);
//    void changeRightDrive(int index);

private:
    Ui::GCMDMainWindow *ui;
};

#endif // GCMDMAINWINDOW_H
