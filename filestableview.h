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
    void copyClicked(const QSet<int>&);
    void moveClicked(const QSet<int>&);
    void newTabCombinationClicked();
    void closeTabCombinationClicked();

public slots:
    void copySelected();
    void moveSelected();
    void deleteSelected();

private:
    void keyPressEvent(QKeyEvent* event) override;

    QSet<int> getSelectedIndexes();

};

#endif // FILESTABLEVIEW_H
