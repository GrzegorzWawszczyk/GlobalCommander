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

private slots:
    void changeDrive(int index);

private:
    Ui::FilesView *ui;
};

#endif // FILESVIEW_H
