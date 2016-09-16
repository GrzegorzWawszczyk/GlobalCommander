#ifndef FILEOPERATIONSHANDLER_H
#define FILEOPERATIONSHANDLER_H

#include "filelistmodel.h"

#include <QFileInfoList>
#include <QMutex>
#include <QWaitCondition>



class FileOperationsHandler : public QObject
{
    Q_OBJECT

public:
    FileOperationsHandler(QObject* parent = 0);

    static void deleteFiles(const QFileInfoList&);
    static void mkdir(const QString& path);

};

#endif // FILEOPERATIONSHANDLER_H
