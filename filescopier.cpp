#include "fileoperationshandler.h"
#include "filescopier.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>


FilesCopier::FilesCopier(QFileInfoList &sourceFiles, QString destPath)
    :   sourceFiles(std::move(sourceFiles)),
        destPath(destPath)
{
    yesToAll = false;
    noToAll = false;
}

void FilesCopier::run()
{
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
                int response;
                emit fileExists(response, source.fileName());

                if (response == QMessageBox::YesToAll)
                {
                    yesToAll = true;
                    QFile::remove(destination);
                    QFile::copy(source.filePath(), destination);
                }
                else if (response == QMessageBox::Yes)
                {
                    QFile::remove(destination);
                    QFile::copy(source.filePath(), destination);
                }
                else if (response == QMessageBox::NoToAll)
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
                int response;
                emit dirExists(response, dir.dirName());

                if (response == QMessageBox::YesToAll)
                {
                    yesToAll = true;
                    copy(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
                }
                else if (response == QMessageBox::Yes)
                {
                    copy(dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot), destDir.path());
                }
                else if (response == QMessageBox::NoToAll)
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
