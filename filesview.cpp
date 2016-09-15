#include "drivelistmodel.h"
#include "filelistmodel.h"
#include "filesview.h"
#include "ui_filesview.h"

#include <QDir>
#include <QStorageInfo>
#include <QMessageBox>
#include <QShortcut>
#include <QDebug>

FilesView::FilesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilesView)
{
    ui->setupUi(this);

    ui->tv_files->setModel(new FileListModel(this));

    QFileInfoList drives = QDir::drives();

    ui->l_path->setText(drives.at(0).absolutePath());

    currentDrivePath = drives.at(0).absolutePath();

    setSpaceInfo();

    ui->cb_drive->setModel(new DriveListModel(this));

    ui->tv_files->setSelectionBehavior(QAbstractItemView::SelectRows);

//    QHeaderView* vh = ui->tv_files->verticalHeader();

    QHeaderView* hh =  ui->tv_files->horizontalHeader();
    hh->setSectionResizeMode(FileListModel::Columns::Icon, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Extension, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Size, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Date, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Attributes, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Name, QHeaderView::Stretch);

    connect(ui->cb_drive, &DrivesComboBox::clicked, dynamic_cast<DriveListModel*>(ui->cb_drive->model()), &DriveListModel::changeDriveList);

    connect(ui->cb_drive, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDrive(int)));

    connect(ui->tv_files, &QTableView::activated, dynamic_cast<FileListModel*>(ui->tv_files->model()), &FileListModel::handleActivate);

    connect(dynamic_cast<FileListModel*>(ui->tv_files->model()), &FileListModel::directoryChanged, this, &FilesView::changePathLabelAndSpaceLabel);

    connect(dynamic_cast<FileListModel*>(ui->tv_files->model()), &FileListModel::fileActivated, this, &FilesView::showInfo);  // DELETE LATER !!!

    connect(ui->tv_files, &FilesTableView::backspaceClicked, dynamic_cast<FileListModel*>(ui->tv_files->model()), &FileListModel::cdUp);
    connect(ui->tv_files, &FilesTableView::deleteClicked, dynamic_cast<FileListModel*>(ui->tv_files->model()), &FileListModel::deleteFile);
}

FilesView::~FilesView()
{
    delete ui;
}

void FilesView::changePathLabelAndSpaceLabel(const QString& path)
{
    ui->l_path->setText(path);
    setSpaceInfo();
}

void FilesView::showInfo()
{
    QMessageBox::information(this, tr("Can't open files"), tr("Sorry, I still can't open files :("));  // DELETE LATER !!!
}

void FilesView::changeDrive(int index)
{
    if (index >=0 && ui->cb_drive->currentText() != currentDrivePath)
    {
        dynamic_cast<FileListModel*>(ui->tv_files->model())->setDirectory(ui->cb_drive->currentText());
        currentDrivePath = ui->cb_drive->currentText();
    }
}

void FilesView::setSpaceInfo()
{
    qint64 totalSpace = QStorageInfo(currentDrivePath).bytesTotal();

    qint64 freeSpace = QStorageInfo(currentDrivePath).bytesFree();

    ui->l_space->setText(QString::number(freeSpace/1024/1024) + "MB / " + QString::number(totalSpace/1024/1024) + "MB");
}


