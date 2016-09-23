#include "gcmdmainwindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QCoreApplication::setOrganizationName("QtGL");
    QCoreApplication::setApplicationName("Global Commander");

    GCMDMainWindow w;
//    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
