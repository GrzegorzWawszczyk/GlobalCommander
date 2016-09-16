#ifndef FILESTABLEVIEW_H
#define FILESTABLEVIEW_H

#include <QKeyEvent>
#include <QTableView>
#include <QObject>



class FilesTableView : public QTableView
{
    Q_OBJECT

public:
    explicit FilesTableView(QWidget* parent = 0);

signals:
    void backspaceClicked();
    void deleteClicked(const QSet<int>&);
    void F5Clicked(const QSet<int>&);
    void F6Clicked(const QSet<int>&);
    void F7Clicked();
    void tabClicked();

private:
    void keyPressEvent(QKeyEvent* event) override;

};

#endif // FILESTABLEVIEW_H
