#include "gcmdmainwindow.h"
#include "ui_gcmdmainwindow.h"

#include <QComboBox>
#include <QDebug>
#include <QFileInfoList>
#include <QStorageInfo>

GCMDMainWindow::GCMDMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GCMDMainWindow)
{
    ui->setupUi(this);

        //connect(ui->cb_disk, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDrive(int)));

    QFileInfoList drives = QDir::drives();

    for (QFileInfo driveInfo : drives){
        qDebug() << driveInfo.absolutePath();
        ui->cb_drive_left->addItem(driveInfo.absolutePath());
        ui->cb_drive_right->addItem(driveInfo.absolutePath());
    }

}

GCMDMainWindow::~GCMDMainWindow()
{
    delete ui;
}

void GCMDMainWindow::changeDrive(int index)
{
    //qDebug() << ui->cb_disk->currentText();
    //qDebug() << index;
}
