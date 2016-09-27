#ifndef FILESVIEW_H
#define FILESVIEW_H

#include <QFileInfoList>
#include <QWidget>

namespace Ui {
class FilesView;
}

class FilesView : public QWidget
{
    Q_OBJECT

public:
    explicit FilesView(QWidget *parent = 0);
    ~FilesView();
    void setSecondView(FilesView*);
    void setDirectory(const QString& path);
    void setDrive(const QString &path);
    void refreshView();

signals:
    void actionCopyClicked();
    void actionMoveClicked();
    void actionDeleteClicked();
    void pathChanged(const QString& pathName, FilesView* exactView); //Couldn't use const for pointer
    void newTabCombinationClicked(const QString& path);
    void closeTabCombinationClicked();
    void deletedDirectory(const QString& path);
    void openListerRequested(const QStringList& paths);//prepared for multiple items handling in future

public slots:
    void changePathLabelAndSpaceLabel(const QFileInfo&);
    void showInfo();  // DELETE LATER !!!
    void checkIfInDeletedDirectory(const QString& path);
    void handleActionCopyClick();
    void handleActionMoveClick();
    void handleActionDeleteClick();
    void handleActionCreateDirClick();

private slots:
    void changeDrive(int);
    void copyFiles(const QSet<int> indexes);
    void moveFiles(const QSet<int> indexes);
    void makeDir();
    void checkIfOtherViewsInDeletedDirectory(const QString& path);
    void changeFocus();
    void listFile(const QSet<int>& indexes);

private:
    Ui::FilesView *ui;

    FilesView* secondView;

    QString currentDrivePath;
    void setSpaceInfo();
    void focusInEvent(QFocusEvent* event) override;

    void loadSettings();
};

#endif // FILESVIEW_H
