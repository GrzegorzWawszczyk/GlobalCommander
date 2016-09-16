#include "drivelistmodel.h"
#include "filelistmodel.h"
#include "fileoperationshandler.h"
#include "filesview.h"
#include "ui_filesview.h"

#include <QDir>
#include <QStorageInfo>
#include <QMessageBox>
#include <QShortcut>
#include <QDebug>
#include <QInputDialog>

FilesView::FilesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilesView)
{
    ui->setupUi(this);

    FileListModel* flm = new FileListModel(this);


    connect(flm, &FileListModel::directoryChanged, this, &FilesView::changePathLabelAndSpaceLabel);
    connect(ui->tv_files, &QTableView::activated, flm, &FileListModel::handleActivate);
    connect(flm, &FileListModel::fileActivated, this, &FilesView::showInfo);  // DELETE LATER !!!
    connect(ui->tv_files, &FilesTableView::backspaceClicked, flm, &FileListModel::cdUp);
    connect(ui->tv_files, &FilesTableView::deleteClicked, flm, &FileListModel::deleteFiles);
    connect(ui->tv_files, &FilesTableView::F5Clicked, this, &FilesView::copyFiles);
    connect(ui->tv_files, &FilesTableView::F6Clicked, this, &FilesView::moveFiles);
    connect(ui->tv_files, &FilesTableView::F7Clicked, this, &FilesView::makeDir);
    connect(ui->tv_files, &FilesTableView::tabClicked, this, &FilesView::changeFocus);
    connect(flm, &FileListModel::deletingDirectory, this, &FilesView::checkIfSecondViewInDeletedDirectory);

    ui->tv_files->setModel(flm);
    ui->tv_files->setSelectionBehavior(QAbstractItemView::SelectRows);


    QFileInfoList drives = QDir::drives();
    ui->l_path->setText(drives.first().absolutePath());
    currentDrivePath = drives.first().absolutePath();

    setSpaceInfo();

    DriveListModel* dlm = new DriveListModel(this);
    connect(ui->cb_drive, &DrivesComboBox::clicked, dlm, &DriveListModel::changeDriveList);
    connect(ui->cb_drive, SIGNAL(clicked()), this, SLOT(setFocus()));
    ui->cb_drive->setModel(dlm);
    connect(ui->cb_drive, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDrive(int)));


//    QHeaderView* vh = ui->tv_files->verticalHeader();

    QHeaderView* hh =  ui->tv_files->horizontalHeader();
    hh->setSectionResizeMode(FileListModel::Columns::Icon, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Extension, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Size, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Date, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Attributes, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Name, QHeaderView::Stretch);

}

FilesView::~FilesView()
{
    delete ui;
}

void FilesView::setSecondView(FilesView *pointer)
{
    secondView = pointer;
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

void FilesView::checkIfInDeletedDirectory(const QString &path)
{
    dynamic_cast<FileListModel*>(ui->tv_files->model())->checkIfInDeletedDirectory(path);
}

void FilesView::changeDrive(int index)
{
    if (index >=0 && ui->cb_drive->currentText() != currentDrivePath)
    {
        dynamic_cast<FileListModel*>(ui->tv_files->model())->setDirectory(ui->cb_drive->currentText());
        currentDrivePath = ui->cb_drive->currentText();
    }
}

void FilesView::copyFiles(const QSet<int> indexes)
{
    dynamic_cast<FileListModel*>(ui->tv_files->model())->copyFiles(indexes, dynamic_cast<FileListModel*>(secondView->ui->tv_files->model())->getCurrentPath());
}

void FilesView::moveFiles(const QSet<int> indexes)
{
    dynamic_cast<FileListModel*>(ui->tv_files->model())->moveFiles(indexes, dynamic_cast<FileListModel*>(secondView->ui->tv_files->model())->getCurrentPath());
}

void FilesView::makeDir()
{
    QString dirName = QInputDialog::getText(nullptr, tr("Type directory name"), tr("Directory name"), QLineEdit::Normal);
    FileOperationsHandler::mkdir(dynamic_cast<FileListModel*>(ui->tv_files->model())->getCurrentPath() + QDir::separator() + dirName);
}

void FilesView::checkIfSecondViewInDeletedDirectory(const QString &path)
{
    secondView->checkIfInDeletedDirectory(path);
}

void FilesView::changeFocus()
{
    secondView->setFocus();
}

void FilesView::setSpaceInfo()
{
    qint64 totalSpace = QStorageInfo(currentDrivePath).bytesTotal();

    qint64 freeSpace = QStorageInfo(currentDrivePath).bytesFree();

    ui->l_space->setText(QString::number(freeSpace/1024/1024) + "MB / " + QString::number(totalSpace/1024/1024) + "MB");
}

void FilesView::focusInEvent(QFocusEvent *event)
{
    ui->tv_files->setFocus();
}


