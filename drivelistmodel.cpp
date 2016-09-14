#include "drivelistmodel.h"

#include <QDebug>
#include <QDir>
#include <QFileInfoList>

DriveListModel::DriveListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QFileInfoList drives = QDir::drives();

        for (QFileInfo driveInfo : drives)
        {
            driveList.append(driveInfo.absolutePath());
        }
}

int DriveListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return driveList.size();
}

QVariant DriveListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= driveList.size() || role != Qt::DisplayRole)
        return QVariant();

    // FIXME: Implement me!
    return driveList.at(index.row());
}

void DriveListModel::changeDriveList()
{
    beginResetModel();

       driveList.clear();
       QFileInfoList drives = QDir::drives();

       for (QFileInfo driveInfo : drives)
       {
           driveList.append(driveInfo.absolutePath());
       }

       endResetModel();

}
