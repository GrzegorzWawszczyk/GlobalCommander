#include "fileoperationshandler.h"

#include <QMessageBox>
#include <QtConcurrent>

FileOperationsHandler::FileOperationsHandler(QObject* parent) : QObject(parent)
{

}


void FileOperationsHandler::deleteFiles(const QFileInfoList &files)
{
    QtConcurrent::run([](const QFileInfoList& files){

        for (const QFileInfo& file : files)
        {
            if (!file.isDir())
                QFile::remove(file.filePath());
            else
                QDir(file.filePath()).removeRecursively();
        }
    }, files);
}

void FileOperationsHandler::mkdir(const QString &path)
{
    QDir().mkpath(path);
}
