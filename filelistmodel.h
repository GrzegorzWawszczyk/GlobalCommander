#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>

class FileListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum Columns
    {
        Name,
        Extension,
        Size,
        Date,
        Count
    };

    explicit FileListModel(QObject *parent = 0);
    ~FileListModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    void setDirectory(const QString& path);

public slots:
    void setDirectoryByIndex(const QModelIndex& index);

private:
    QFileInfoList files;
};

#endif // FILELISTMODEL_H
