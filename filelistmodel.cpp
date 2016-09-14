#include "filelistmodel.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent)
{

    QFileInfoList drives = QDir::drives();

    setDirectory(drives.at(0).absolutePath());
}

FileListModel::~FileListModel()
{

}

QVariant FileListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
        switch (section)
        {
        case Columns::Name:
            return "Name";
        case Columns::Extension:
            return "Ext";
        case Columns::Size:
            return "Size";
        case Columns::Date:
            return "Date";
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
    if (!index.isValid() || index.row() >= files.size() || role != Qt::DisplayRole)
        return QVariant();

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
        return file.suffix();

    case Columns::Size:
        if (file.isDir())
            return "<DIR>";
        return file.size();

    case Columns::Date:
        return file.lastModified();
    }

    // FIXME: Implement me!
    return QVariant();
}

void FileListModel::setDirectory(const QString& path)
{
    beginResetModel();

    QDir dir;
    dir.setPath(path);
    files = dir.entryInfoList(QDir::AllEntries | QDir::NoDot, QDir::DirsFirst | QDir::IgnoreCase);

    endResetModel();
}

void FileListModel::setDirectoryByIndex(const QModelIndex &index)
{
    setDirectory(files.at(index.row()).absoluteFilePath());
}
