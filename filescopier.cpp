#include "fileoperationshandler.h"
#include "filescopier.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>


FilesCopier::FilesCopier(QFileInfoList &sourceFiles, QString destPath, const QMessageBox::StandardButton &decision)
    :   sourceFiles(std::move(sourceFiles)),
        destPath(destPath),
        decision(decision)
{
    yesToAll = false;
    noToAll = false;
}

void FilesCopier::run()
{
    if (sourceFiles.first().dir().path() == destPath)
        return;

    copy(sourceFiles, destPath);
}

void FilesCopier::copy(const QFileInfoList &filesToCopy, const QString &path)
{
    for (const QFileInfo& source : filesToCopy)
    {
        QString destination = path + QDir::separator() + source.fileName();
        if (source.isFile())
        {
            if (QFileInfo(destination).exists() && yesToAll)
            {
                QFile::remove(destination);
                QFile::copy(source.filePath(), destination);
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
                    QFile::copy(source.filePath(), destination);
                }
                else if (decision == QMessageBox::Yes)
                {
                    QFile::remove(destination);
                    QFile::copy(source.filePath(), destination);
                }
                else if (decision == QMessageBox::NoToAll)
                    noToAll = true;
            }
            else
            {
                QFile::copy(source.filePath(), destination);
            }
        }
        else
        {
            QDir dir = QDir(source.filePath());
            QDir destDir = QDir(destination);
            if (destDir.exists() && yesToAll)
            {
                copy(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
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
                    copy(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
                }
                else if (decision == QMessageBox::Yes)
                {
                    copy(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
                }
                else if (decision == QMessageBox::NoToAll)
                    noToAll = true;
            }
            else
            {
                QDir().mkpath(destDir.path());
                copy(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
            }
        }
    }
}
