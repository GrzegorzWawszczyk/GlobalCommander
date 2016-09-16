#include "filesmover.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>

FilesMover::FilesMover(QFileInfoList &sourceFiles, QString destPath)
    :   sourceFiles(std::move(sourceFiles)),
        destPath(destPath)
{
    yesToAll = false;
    noToAll = false;
}

void FilesMover::run()
{
    move(sourceFiles, destPath);
}

void FilesMover::move(const QFileInfoList &filesToMove, const QString &path)
{
    for (const QFileInfo& source : filesToMove)
    {
        QString destination = path + QDir::separator() + source.fileName();
        if (source.isFile())
        {
            if (QFileInfo(destination).exists() && yesToAll)
            {
                QFile::remove(destination);
                QFile::rename(source.filePath(), destination);
            }
            else if (QFileInfo(destination).exists() && noToAll)
                continue;

            else if (QFileInfo(destination).exists())
            {
                int response;
                emit fileExists(response, source.fileName());

                if (response == QMessageBox::YesToAll)
                {
                    yesToAll = true;
                    QFile::remove(destination);
                    QFile::rename(source.filePath(), destination);
                }
                else if (response == QMessageBox::Yes)
                {
                    QFile::remove(destination);
                    QFile::rename(source.filePath(), destination);
                }
                else if (response == QMessageBox::NoToAll)
                    noToAll = true;
            }
            else
            {
                QFile::rename(source.filePath(), destination);
            }
        }
        else
        {
            QDir dir = QDir(source.filePath());
            QDir destDir = QDir(destination);
            if (destDir.exists() && yesToAll)
            {
                move(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
            }
            else if (destDir.exists() && noToAll)
            {
                continue;
            }
            else if (destDir.exists())
            {
                int response;
                emit dirExists(response, dir.dirName());

                if (response == QMessageBox::YesToAll)
                {
                    yesToAll = true;
                    move(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
                }
                else if (response == QMessageBox::Yes)
                {
                    move(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
                }
                else if (response == QMessageBox::NoToAll)
                    noToAll = true;
            }
            else
            {
                QDir().rename(dir.path(), destDir.path());
            }
        }
    }
}
