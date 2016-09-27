#ifndef FILESTABLEVIEW_H
#define FILESTABLEVIEW_H

#include <QKeyEvent>
#include <QTableView>
#include <QObject>
#include <QSettings>



class FilesTableView : public QTableView
{
    Q_OBJECT

public:
    explicit FilesTableView(QWidget* parent = 0);

signals:
    void backspaceClicked();
    void deleteClicked(const QSet<int>& indexes);
    void F3Clicked(const QSet<int>& indexes);//prepared for multiple items handling in future
    void F5Clicked(const QSet<int>& indexes);
    void F6Clicked(const QSet<int>& indexes);
    void F7Clicked();
    void tabClicked();
    void copyClicked(const QSet<int>& indexes);
    void moveClicked(const QSet<int>& indexes);
    void newTabCombinationClicked();
    void closeTabCombinationClicked();

public slots:
    void copySelected();
    void moveSelected();
    void deleteSelected();

private:
    void keyPressEvent(QKeyEvent* event) override;

    QSet<int> getSelectedIndexes();

    QSettings settings;

};

#endif // FILESTABLEVIEW_H
