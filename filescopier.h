#ifndef FILECOPIER_H
#define FILECOPIER_H

#include <QFileInfoList>
#include <QThread>



class FilesCopier : public QThread
{
    Q_OBJECT

public:
    FilesCopier(QFileInfoList& sourceFiles, QString destPath);

    void run() override;

signals:
    void fileExists(int& response, const QString& fileName);
    void dirExists(int& response, const QString& dirName);

private:
    QFileInfoList sourceFiles;
    QString destPath;
    bool yesToAll;
    bool noToAll;

    void copy(const QFileInfoList& filesToCopy, const QString& path);
};

#endif // FILECOPIER_H
