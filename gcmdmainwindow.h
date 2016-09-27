#ifndef GCMDMAINWINDOW_H
#define GCMDMAINWINDOW_H

#include "filesview.h"
#include "settingswindow.h"

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class GCMDMainWindow;
}

class GCMDMainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit GCMDMainWindow(QWidget *parent = 0);
    ~GCMDMainWindow();

public slots:
    void addTabLeft(const QDir& path);
    void addTabRight(const QDir& path);

private slots:
    void changeLeftTabText(const QString& pathName, FilesView *exactView); //Couldn't use const for pointer
    void changeRightTabText(const QString& pathName, FilesView *exactView); //Couldn't use const for pointer
    void checkIfLeftViewsInDeletedDirectory(const QString& path);
    void checkIfRightViewsInDeletedDirectory(const QString& path);
    void openLister(const QStringList &paths);

private:
    Ui::GCMDMainWindow *ui;
};

#endif // GCMDMAINWINDOW_H
