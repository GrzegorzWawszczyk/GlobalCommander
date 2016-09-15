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

public slots:
    void changePathLabelAndSpaceLabel(const QString &path);
    void showInfo();  // DELETE LATER !!!

private slots:
    void changeDrive(int index);

private:
    Ui::FilesView *ui;

    QString currentDrivePath;
    void setSpaceInfo();
};

#endif // FILESVIEW_H
