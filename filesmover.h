#ifndef FILEMOVER_H
#define FILEMOVER_H

#include <QFileInfoList>
#include <QThread>
#include <QMessageBox>



class FilesMover : public QThread
{
    Q_OBJECT

public:
    FilesMover(QFileInfoList &sourceFiles, QString destPath, const QMessageBox::StandardButton& decision);

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

    void move(const QFileInfoList& filesToMove, const QString& path);
};

#endif // FILEMOVER_H
