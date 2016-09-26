#ifndef FILECOPIER_H
#define FILECOPIER_H

#include <QFileInfoList>
#include <QMessageBox>
#include <QThread>



class FilesCopier : public QThread
{
    Q_OBJECT

public:
    FilesCopier(QFileInfoList& sourceFiles, QString destPath, const QMessageBox::StandardButton &decision);

    void run() override;

signals:
    void fileExists(const QString& fileName);
    void dirExists(const QString& dirName);

private:
    QFileInfoList sourceFiles;
    QString destPath;
    bool yesToAll;
    bool noToAll;
    const QMessageBox::StandardButton& decision;

    void copy(const QFileInfoList& filesToCopy, const QString& path);
};

#endif // FILECOPIER_H
