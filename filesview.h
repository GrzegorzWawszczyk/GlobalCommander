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

signals:
    void actionCopyClicked();
    void actionMoveClicked();
    void actionDeleteClicked();

public slots:
    void changePathLabelAndSpaceLabel(const QString&);
    void showInfo();  // DELETE LATER !!!
    void checkIfInDeletedDirectory(const QString& path);
    void handleActionCopyClick();
    void handleActionMoveClick();
    void handleActionDeleteClick();

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
