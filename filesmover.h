#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QFileInfoList>
#include <QThread>



class FilesMover : public QThread
{
    Q_OBJECT

public:
    FilesMover(QFileInfoList &sourceFiles, QString destPath);

    void run() override;

signals:
    void fileExists(int& response, const QString& fileName);
    void dirExists(int& response, const QString& dirName);

private:
    QFileInfoList sourceFiles;
    QString destPath;
    bool yesToAll;
    bool noToAll;

    void move(const QFileInfoList& filesToMove, const QString& path);
};

#endif // FILEMOVER_H
