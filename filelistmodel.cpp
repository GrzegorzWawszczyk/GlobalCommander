#include "filescopier.h"
#include "filelistmodel.h"
#include "fileoperationshandler.h"
#include "filesmover.h"
#include "settingswindow.h"
#include "gcmdmainwindow.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileIconProvider>
#include <QFont>
#include <QMessageBox>
#include <QMutex>
#include <QWaitCondition>

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent),
      lastFileOverwriteDecision(QMessageBox::No)
{

    setDirectory(QDir::drives().first().absolutePath());

    watcher.addPath(QDir::drives().first().absolutePath());

    connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &FileListModel::refreshDirectory);

    loadSettings();
}


QVariant FileListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) //&& role == Qt::DisplayRole)
    {
        if (role == Qt::DisplayRole)
        {
            switch (section)
            {
            case Columns::Name:
                return tr("Name");
            case Columns::Extension:
                return tr("Ext");
            case Columns::Size:
                return tr("Size");
            case Columns::Date:
                return tr("Date");
            case Columns::Attributes:
                return tr("Atrr");
            }
        }
        if (role == Qt::FontRole)
        {
            return headerFont;
        }
    }

    return QVariant();
}

int FileListModel::rowCount(const QModelIndex &parent) const
{
    return files.size();
}

int FileListModel::columnCount(const QModelIndex &parent) const
{
    return Columns::Count;
}

bool FileListModel::insertRows(int row, int count, const QModelIndex &/*parent*/)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    files.insert(files.begin() + row, QFileInfo());
    endInsertRows();

    return true;
}

bool FileListModel::removeRows(int row, int count, const QModelIndex &/*parent*/)
{
    beginRemoveRows(QModelIndex(), row, row + count-1);
    files.erase(files.begin() + row, files.begin()+row+count);
    endRemoveRows();

    return true;
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= files.size())
        return QVariant();

    if (role == Qt::FontRole)
    {
        return fileListFont;
    }

    else if (role == Qt::DisplayRole)
    {
        const QFileInfo& file = files.at(index.row());
        switch (index.column())
        {

        case Columns::Name:
            if (file.isDir())
                return "[" + file.fileName() + "]";
            return file.fileName();

        case Columns::Extension:
            if (file.isDir())
                return "";
            return file.completeSuffix();

        case Columns::Size:
            if (file.isDir())
                return "<DIR>";
            return file.size();

        case Columns::Date:
            return file.lastModified();            

        case Columns::Attributes:
            QString str = "";
            str = str + (file.permission(QFile::ReadUser) ? "r" : "-") +
                    (file.permission(QFile::WriteUser) ? "w" : "-") +
                    (file.permission(QFile::ExeUser) ? "x" : "-") +
                    (file.permission(QFile::ReadGroup) ? "r" : "-") +
                    (file.permission(QFile::WriteGroup) ? "w" : "-") +
                    (file.permission(QFile::ExeGroup) ? "x" : "-") +
                    (file.permission(QFile::ReadOther) ? "r" : "-") +
                    (file.permission(QFile::WriteOther) ? "w" : "-") +
                    (file.permission(QFile::ExeOther) ? "x" : "-");
            return str;

        }
    }

    else if (role == Qt::DecorationRole && index.column() == Columns::Icon)
    {
        const QFileInfo& file = files.at(index.row());
        QFileIconProvider fip;
        return fip.icon(file);
    }

    return QVariant();
}

void FileListModel::setDirectory(const QString& path)
{
    beginResetModel();
    watcher.removePath(currentPath);
    watcher.addPath(path);

    currentPath = path;

    QDir dir(path);

    files = dir.entryInfoList(QDir::AllEntries | QDir::NoDot, QDir::DirsFirst | QDir::IgnoreCase);
    emit directoryChanged(QFileInfo(path));

    endResetModel();
}

QString FileListModel::getCurrentPath()
{
    return currentPath;
}

void FileListModel::handleActivate(const QModelIndex &index)
{
    if (files.at(index.row()).isDir())
        setDirectory(files.at(index.row()).absoluteFilePath());
    else
        emit fileActivated();
}

void FileListModel::cdUp()
{
    QDir dir;
    dir.setPath(currentPath);
    if (!dir.isRoot())
    {
        dir.cdUp();
        setDirectory(dir.absolutePath());
    }
}

void FileListModel::deleteFiles(QSet<int> indexes)
{
    if (!QDir(currentPath).isRoot() && indexes.contains(0))
        indexes.remove(0);

    QString message = indexes.size() == 1
            ? tr("Are you sure you want to delete this?")
            : tr("Are you sure you want to delete %1 selected items?").arg(indexes.size());

    if (QMessageBox::question(nullptr, tr("Confirm delete"), message) != QMessageBox::Yes)
    {
        return;
    }


    QFileInfoList filesToRemove;
    for (int index : indexes)
    {
        if (files.at(index).isDir())
            emit deletingDirectory(files.at(index).filePath());
        filesToRemove.append(files.at(index));
    }
    FileOperationsHandler::deleteFiles(filesToRemove);
}

void FileListModel::copyFiles(QSet<int> indexes, QString path)
{
    if (!QDir(currentPath).isRoot() && indexes.contains(0))
        indexes.remove(0);

    if (indexes.count() == 0)
        return;

    QFileInfoList sourceFiles;
     for (int index : indexes)
     {
         sourceFiles.append(files.at(index));
     }
     FilesCopier* copier = new FilesCopier(sourceFiles, path, lastFileOverwriteDecision);
     connect(copier, &FilesCopier::finished, copier, &FilesCopier::deleteLater);
     connect(copier, &FilesCopier::fileExists, this, &FileListModel::checkOverwrite, Qt::BlockingQueuedConnection);
     connect(copier, &FilesCopier::dirExists, this, &FileListModel::checkMerge, Qt::BlockingQueuedConnection);
     copier->start();
}

void FileListModel::moveFiles(QSet<int> indexes, const QString &path)
{
    if (!QDir(currentPath).isRoot() && indexes.contains(0))
        indexes.remove(0);

    if (indexes.count() == 0)
        return;

    QFileInfoList sourceFiles;
    for (int index : indexes)
    {
        sourceFiles.append(files.at(index));
    }
    FilesMover* mover = new FilesMover(sourceFiles, path, lastFileOverwriteDecision);
    connect(mover, &FilesMover::finished, mover, &FilesMover::deleteLater);
    connect(mover, &FilesMover::fileExists, this, &FileListModel::checkOverwrite, Qt::BlockingQueuedConnection);
    connect(mover, &FilesMover::dirExists, this, &FileListModel::checkMerge, Qt::BlockingQueuedConnection);
    mover->start();
}

void FileListModel::checkIfInDeletedDirectory(const QString &path)
{
    while (currentPath.contains(path))
        cdUp();
}

void FileListModel::refreshView()
{
    beginResetModel();
    loadSettings();
    endResetModel();
}


void FileListModel::refreshDirectory()
{
    setDirectory(currentPath);
}

void FileListModel::checkOverwrite(const QString &fileName)
{
    lastFileOverwriteDecision = QMessageBox::question(nullptr, tr("Confirm overwrite"),
    tr("Do you want to overwrite ") + fileName + "?", QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll );
}

void FileListModel::checkMerge(const QString& dirName)
{
    lastFileOverwriteDecision = QMessageBox::question(nullptr, tr("Confirm overwrite"),
    tr("Do you want to merge ") + dirName + "?", QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::NoToAll );
}

void FileListModel::loadSettings()
{
    headerFont = SettingsWindow::getHeaderFont();
    fileListFont = SettingsWindow::getFileListFont();
}
