#include "gcmdmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GCMDMainWindow w;
    w.show();

    return a.exec();
}
