#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QAbstractListModel>
#include <QFileInfoList>
#include <QFileSystemWatcher>

class FileListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum Columns
    {
        Icon,
        Name,
        Extension,
        Size,
        Date,
        Attributes,
        Count
    };

    explicit FileListModel(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    void setDirectory(const QString& path);
    QString getCurrentPath();
    void copyFiles(QSet<int> indexes, QString path);
    void moveFiles(QSet<int> indexes, const QString& path);
    void checkIfInDeletedDirectory(const QString &path);

public slots:
    void handleActivate(const QModelIndex& index);
    void cdUp();
    void deleteFiles(QSet<int> indexes);

private slots:
    void refreshDirectory();
    void checkOverwrite(int& response, const QString& fileName);
    void checkMerge(int& response, const QString &dirName);

signals:
    void directoryChanged(const QFileInfo& path);
    void fileActivated();
    void deletingDirectory(const QString& path);

private:    
    QFileInfoList files;
    QString currentPath;
    QFileSystemWatcher watcher;
    int fontSize;

    bool copyDirectory(const QString &sourcePath, const QString &destinationPath, bool &yesToAll);
    bool moveDirectory(const QString &sourcePath, const QString &destinationPath, bool &yesToAll);
    bool copyFiles(QFileInfoList &filesToCopy, const QString &path, bool &yesToAll);
    bool moveFiles(QFileInfoList &filesToMove, const QString &path, bool &yesToAll);
};

#endif // FILELISTMODEL_H
