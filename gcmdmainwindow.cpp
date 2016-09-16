#include "drivelistmodel.h"
#include "filelistmodel.h"
#include "gcmdmainwindow.h"
#include "ui_gcmdmainwindow.h"

#include <QComboBox>
#include <QDebug>
#include <QFileInfoList>
#include <QStorageInfo>
#include <filesview.h>

GCMDMainWindow::GCMDMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GCMDMainWindow)
{
    ui->setupUi(this);

    dynamic_cast<FilesView*>(ui->fv_left)->setSecondView(ui->fv_right);
    dynamic_cast<FilesView*>(ui->fv_right)->setSecondView(ui->fv_left);
}

GCMDMainWindow::~GCMDMainWindow()
{
    delete ui;
}
