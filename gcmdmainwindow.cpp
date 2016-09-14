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
}

GCMDMainWindow::~GCMDMainWindow()
{
    delete ui;
}
