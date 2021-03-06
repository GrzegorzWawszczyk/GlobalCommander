#include "drivelistmodel.h"

#include <QDebug>
#include <QDir>
#include <QFileIconProvider>
#include <QFileInfoList>

DriveListModel::DriveListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    refreshDriveList();
}

int DriveListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return driveList.size();
}

QVariant DriveListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= driveList.size() || (role != Qt::DisplayRole && role != Qt::DecorationRole))
        return QVariant();

    if (role == Qt::DisplayRole)
        return driveList.at(index.row());
    if (role == Qt::DecorationRole){
        QFileIconProvider provider;

        return provider.icon(QFileIconProvider::Drive);
    }
}

void DriveListModel::changeDriveList()
{

    beginResetModel();

    refreshDriveList();

    endResetModel();

}

void DriveListModel::refreshDriveList()
{
    driveList.clear();

    for (QFileInfo driveInfo : QDir::drives())
    {
        driveList.append(driveInfo.absolutePath());
    }
}
