#include "drivelistmodel.h"
#include "filelistmodel.h"
#include "filesview.h"
#include "ui_filesview.h"

FilesView::FilesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilesView)
{
    ui->setupUi(this);

    ui->tv_files->setModel(new FileListModel(this));

    ui->cb_drive->setModel(new DriveListModel(this));

    connect(ui->cb_drive, &DrivesComboBox::clicked, dynamic_cast<DriveListModel*>(ui->cb_drive->model()), &DriveListModel::changeDriveList);

    connect(ui->cb_drive, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDrive(int)));

    connect(ui->tv_files, &QTableView::activated, dynamic_cast<FileListModel*>(ui->tv_files->model()), &FileListModel::setDirectoryByIndex);
}

FilesView::~FilesView()
{
    delete ui;
}

void FilesView::changeDrive(int index)
{
    if (index >=0 )
        dynamic_cast<FileListModel*>(ui->tv_files->model())->setDirectory(ui->cb_drive->currentText());
}


