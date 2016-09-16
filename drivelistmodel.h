#ifndef DRIVELISTMODEL_H
#define DRIVELISTMODEL_H

#include <QAbstractListModel>

class DriveListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit DriveListModel(QObject *parent = 0);


    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void changeDriveList();

private:
    QStringList driveList;
    void refreshDriveList();

};

#endif // DRIVELISTMODEL_H
