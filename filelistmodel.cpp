#include "filelistmodel.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileIconProvider>
#include <QFont>
#include <QMessageBox>

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent)
{

    QFileInfoList drives = QDir::drives();

    setDirectory(drives.at(0).absolutePath());

    emit directoryChanged(drives.at(0).absolutePath());

    watcher.addPath(drives.at(0).absolutePath());

    connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &FileListModel::refreshDirectory);

    fontSize = 7;
}

FileListModel::~FileListModel()
{

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
            return QFont("Segoe UI", fontSize, QFont::Bold);
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
    beginInsertRows(QModelIndex(), row, row+count-1);
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
        return QFont("Segoe UI", fontSize, QFont::Normal);

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
        return fip.icon(file).pixmap(fontSize * 1.33);
    }

    // FIXME: Implement me!
    return QVariant();
}

void FileListModel::setDirectory(const QString& path)
{
    beginResetModel();

    watcher.removePath(currentPath);
    watcher.addPath(path);
//    qDebug() << watcher.directories();

    currentPath = path;

    QDir dir(path);
    //dir.setPath(path);
    files = dir.entryInfoList(QDir::AllEntries | QDir::NoDot, QDir::DirsFirst | QDir::IgnoreCase);
    emit directoryChanged(path);

    endResetModel();
}

bool FileListModel::removeDirectory(const QString & path)
{
    bool result = true;
       QDir dir(path);
//       dir.setPath(path);

       if (dir.exists()) {
           Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
               if (info.isDir()) {
                   result = removeDirectory(info.absoluteFilePath());
               }
               else {
                   result = QFile::remove(info.absoluteFilePath());
               }

               if (!result) {
                   return result;
               }
           }
           result = QDir().rmdir(path);
       }
       return result;
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

void FileListModel::deleteFile(int index)
{
    if (QMessageBox::question(0, tr("Confirm delete"), tr("Are you sure you want to delete this?")) == QMessageBox::Yes)
    {
        if (!files.at(index).isDir())
            QFile::remove(files.at(index).filePath());
        else
            removeDirectory(files.at(index).filePath());
    }
}

void FileListModel::refreshDirectory()
{
    setDirectory(currentPath);
}
