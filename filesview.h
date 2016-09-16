#ifndef FILESVIEW_H
#define FILESVIEW_H

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

public slots:
    void changePathLabelAndSpaceLabel(const QString&);
    void showInfo();  // DELETE LATER !!!
    void checkIfInDeletedDirectory(const QString& path);

private slots:
    void changeDrive(int);
    void copyFiles(const QSet<int> indexes);
    void moveFiles(const QSet<int> indexes);
    void makeDir();
    void checkIfSecondViewInDeletedDirectory(const QString& path);
    void changeFocus();

private:
    Ui::FilesView *ui;

    FilesView* secondView;

    QString currentDrivePath;
    void setSpaceInfo();
    void focusInEvent(QFocusEvent* event) override;
};

#endif // FILESVIEW_H
