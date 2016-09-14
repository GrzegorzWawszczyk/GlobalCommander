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

//    QFileInfoList drives = QDir::drives();


//    ui->fv_left->setModel(new FileListModel(this));
//    ui->tv_files_right->setModel(new FileListModel(this));

////    DriveListModel *dlm = new DriveListModel(this);
//    ui->cb_drive_left->setModel(new DriveListModel(this));
//    ui->cb_drive_right->setModel(new DriveListModel(this));

//    connect(ui->cb_drive_left, &DrivesComboBox::clicked, dynamic_cast<DriveListModel*>(ui->cb_drive_left->model()), &DriveListModel::changeDriveList);
//    connect(ui->cb_drive_right, &DrivesComboBox::clicked, dynamic_cast<DriveListModel*>(ui->cb_drive_right->model()), &DriveListModel::changeDriveList);

//    dynamic_cast<FileListModel*>(ui->tv_files_left->model())->setDirectory(drives.at(0).absolutePath());
//    dynamic_cast<FileListModel*>(ui->tv_files_right->model())->setDirectory(drives.at(0).absolutePath());

//    connect(ui->cb_drive_left, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLeftDrive(int)));
//    connect(ui->cb_drive_right, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRightDrive(int)));

//    connect(ui->tv_files_left, &QTableView::activated, dynamic_cast<FileListModel*>(ui->tv_files_left->model()), &FileListModel::setDirectoryByIndex);
//    connect(ui->tv_files_right, &QTableView::activated, dynamic_cast<FileListModel*>(ui->tv_files_right->model()), &FileListModel::setDirectoryByIndex);

}

GCMDMainWindow::~GCMDMainWindow()
{
    delete ui;
}

//void GCMDMainWindow::changeLeftDrive(int index)
//{
//    qDebug() << ui->cb_drive_left->currentText();
//    if (index >=0 )
//        dynamic_cast<FileListModel*>(ui->tv_files_left->model())->setDirectory(ui->cb_drive_left->currentText());
//}

//void GCMDMainWindow::changeRightDrive(int index)
//{
//    if (index >=0 )
//        dynamic_cast<FileListModel*>(ui->tv_files_right->model())->setDirectory(ui->cb_drive_right->currentText());
//}


// test comment
