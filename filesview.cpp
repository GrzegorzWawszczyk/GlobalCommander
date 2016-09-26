#include "drivelistmodel.h"
#include "filelistmodel.h"
#include "fileoperationshandler.h"
#include "filesview.h"
#include "settingswindow.h"
#include "ui_filesview.h"

#include <QDir>
#include <QStorageInfo>
#include <QMessageBox>
#include <QShortcut>
#include <QDebug>
#include <QInputDialog>
#include <QBitArray>

FilesView::FilesView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilesView)
{
    ui->setupUi(this);

    //Creating FileListModel and its connects
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
    connect(ui->tv_files, &FilesTableView::newTabCombinationClicked, this, [this](){
       emit newTabCombinationClicked(dynamic_cast<FileListModel*>(ui->tv_files->model())->getCurrentPath());
    });
    connect(ui->tv_files, &FilesTableView::closeTabCombinationClicked, this, &FilesView::closeTabCombinationClicked);
    connect(this, &FilesView::actionCopyClicked, ui->tv_files, &FilesTableView::copySelected);
    connect(ui->tv_files, &FilesTableView::copyClicked, this, &FilesView::copyFiles);
    connect(this, &FilesView::actionMoveClicked, ui->tv_files, &FilesTableView::moveSelected);
    connect(ui->tv_files, &FilesTableView::moveClicked, this, &FilesView::moveFiles);
    connect(this, &FilesView::actionDeleteClicked, ui->tv_files, &FilesTableView::deleteSelected);
    connect(flm, &FileListModel::deletingDirectory, this, &FilesView::checkIfOtherViewsInDeletedDirectory);

    ui->tv_files->setModel(flm);
    ui->tv_files->setSelectionBehavior(QAbstractItemView::SelectRows);
    //*********************************************************************************************************

    //Creating DriveListModel and its connects
    QFileInfoList drives = QDir::drives();
    ui->l_path->setText(drives.first().absolutePath());
    emit pathChanged(drives.first().absolutePath(), this);
    currentDrivePath = drives.first().absolutePath();

    setSpaceInfo();

    DriveListModel* dlm = new DriveListModel(this);
    connect(ui->cb_drive, &DrivesComboBox::clicked, dlm, &DriveListModel::changeDriveList);
    connect(ui->cb_drive, SIGNAL(clicked()), this, SLOT(setFocus()));
    ui->cb_drive->setModel(dlm);
    connect(ui->cb_drive, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDrive(int)));
    //*********************************************************************************************************

    //Setting header
    QHeaderView* hh =  ui->tv_files->horizontalHeader();
    hh->setSectionResizeMode(FileListModel::Columns::Icon, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Extension, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Size, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Date, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Attributes, QHeaderView::ResizeToContents);
    hh->setSectionResizeMode(FileListModel::Columns::Name, QHeaderView::Stretch);

    ui->tv_files->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //*********************************************************************************************************

    //Loading Settings
    loadSettings();
    //******************
}

FilesView::~FilesView()
{
    delete ui;
}

void FilesView::setSecondView(FilesView *pointer)
{
    secondView = pointer; //Pointer on view visible in second panel
}

void FilesView::setDirectory(const QString &path)
{
    dynamic_cast<FileListModel*>(ui->tv_files->model())->setDirectory(path);
}

void FilesView::setDrive(const QString &path)
{
    QDir dir = QDir(path);
    while(dir.cdUp());
    ui->cb_drive->setCurrentText(dir.path());
}

void FilesView::refreshView()
{
    dynamic_cast<FileListModel*>(ui->tv_files->model())->refreshView();
    loadSettings();
}

void FilesView::changePathLabelAndSpaceLabel(const QFileInfo &path)
{
    ui->l_path->setText(path.filePath());
    setSpaceInfo();
    if (!path.isRoot())
        emit pathChanged(path.fileName(), this);
    else
        emit pathChanged(path.path(), this);
}

//Temporary, called when trying to open file
void FilesView::showInfo()
{
    QMessageBox::information(this, tr("Can't open files"), tr("Sorry, I still can't open files :("));  // DELETE LATER !!!
}

//Checking if this view is in deleted directory, to go up to existing directory
void FilesView::checkIfInDeletedDirectory(const QString &path)
{
    dynamic_cast<FileListModel*>(ui->tv_files->model())->checkIfInDeletedDirectory(path);
}

//Handling menu actions triggering
void FilesView::handleActionCopyClick()
{
    emit actionCopyClicked();
}

void FilesView::handleActionMoveClick()
{
    emit actionMoveClicked();
}

void FilesView::handleActionDeleteClick()
{
    emit actionDeleteClicked();
}

void FilesView::handleActionCreateDirClick()
{
    //Checking if this is focused view
    if (ui->tv_files == QApplication::focusWidget())
        makeDir();
}
//**********************************************


void FilesView::changeDrive(int index)
{
    if (index >=0 && ui->cb_drive->currentText() != currentDrivePath)
    {
        currentDrivePath = ui->cb_drive->currentText();
        dynamic_cast<FileListModel*>(ui->tv_files->model())->setDirectory(ui->cb_drive->currentText());        
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
    QString currentPath = dynamic_cast<FileListModel*>(ui->tv_files->model())->getCurrentPath();
    QString dirName = QInputDialog::getText(nullptr, tr("Type directory name"), tr("Directory name"), QLineEdit::Normal, tr("New Folder"));
    while (dirName.isEmpty())
    {
        QMessageBox::warning(nullptr, tr("Empty name"), tr("Directory name can't be empty"));
        dirName = QInputDialog::getText(nullptr, tr("Type directory name"), tr("Directory name"), QLineEdit::Normal, tr("New Folder"));
    }
    QString newDirPath = currentPath + QDir::separator() + dirName;
    int counter = 1;
    while (QDir(newDirPath).exists())
    {
        counter++;
        newDirPath = currentPath + QDir::separator() + dirName + " (" + QString::number(counter) + ")";
    }
    FileOperationsHandler::mkdir(newDirPath);
}

//Checking if other views are in deleted directory, to go up to existing directory
void FilesView::checkIfOtherViewsInDeletedDirectory(const QString &path)
{
    emit deletedDirectory(path);
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

void FilesView::loadSettings()
{
    QBitArray enabledColumns = SettingsWindow::getEnabledColumns();
    ui->tv_files->setColumnHidden(FileListModel::Columns::Icon, !enabledColumns.at(FileListModel::Columns::Icon));
    ui->tv_files->setColumnHidden(FileListModel::Columns::Extension, !enabledColumns.at(FileListModel::Columns::Extension));
    ui->tv_files->setColumnHidden(FileListModel::Columns::Size, !enabledColumns.at(FileListModel::Columns::Size));
    ui->tv_files->setColumnHidden(FileListModel::Columns::Date, !enabledColumns.at(FileListModel::Columns::Date));
    ui->tv_files->setColumnHidden(FileListModel::Columns::Attributes, !enabledColumns.at(FileListModel::Columns::Attributes));
}


