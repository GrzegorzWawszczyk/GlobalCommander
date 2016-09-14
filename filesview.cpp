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

    QFont font = ui->tv_files->font();

    font.setPointSize(10);

    ui->tv_files->setFont(font);

    ui->cb_drive->setModel(new DriveListModel(this));

    ui->tv_files->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tv_files->setShowGrid(false);

    //ui->tv_files->setFrameShape(QFrame::NoFrame);

    QHeaderView* hv =  ui->tv_files->horizontalHeader();
    hv->setSectionResizeMode(FileListModel::Columns::Name, QHeaderView::ResizeToContents);
    hv->setSectionResizeMode(FileListModel::Columns::Name, QHeaderView::Interactive);
//    hv->resizeSection(FileListModel::Columns::Name, 50);
//    hv->setsec

    connect(ui->cb_drive, &DrivesComboBox::clicked, dynamic_cast<DriveListModel*>(ui->cb_drive->model()), &DriveListModel::changeDriveList);

    connect(ui->cb_drive, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDrive(int)));

    connect(ui->tv_files, &QTableView::activated, dynamic_cast<FileListModel*>(ui->tv_files->model()), &FileListModel::handleActivate);
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


