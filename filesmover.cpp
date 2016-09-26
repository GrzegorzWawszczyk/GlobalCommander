#include "filesmover.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>

FilesMover::FilesMover(QFileInfoList &sourceFiles, QString destPath, const QMessageBox::StandardButton &decision)
    :   sourceFiles(std::move(sourceFiles)),
        destPath(destPath),
        decision(decision)
{
    yesToAll = false;
    noToAll = false;
}

void FilesMover::run()
{
    if (sourceFiles.first().dir().path() == destPath)
        return;

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
                emit fileExists(source.fileName());

                if (decision == QMessageBox::YesToAll)
                {
                    yesToAll = true;
                    QFile::remove(destination);
                    QFile::rename(source.filePath(), destination);
                }
                else if (decision == QMessageBox::Yes)
                {
                    QFile::remove(destination);
                    QFile::rename(source.filePath(), destination);
                }
                else if (decision == QMessageBox::NoToAll)
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
                emit dirExists(dir.dirName());

                if (decision == QMessageBox::YesToAll)
                {
                    yesToAll = true;
                    move(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
                }
                else if (decision == QMessageBox::Yes)
                {
                    move(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
                }
                else if (decision == QMessageBox::NoToAll)
                    noToAll = true;
            }
            else
            {
                QDir().rename(dir.path(), destDir.path());
            }
        }
    }
}
