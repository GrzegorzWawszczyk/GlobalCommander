#include "filestableview.h"

#include <QDebug>

FilesTableView::FilesTableView(QWidget *parent)
    : QTableView(parent)
{

}

void FilesTableView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace)
        emit backspaceClicked();
    else if (event->key() == Qt::Key_Delete)
    {
        QSet<int> indexes;

        for (QModelIndex index: selectedIndexes())
            indexes.insert(index.row());

        emit deleteClicked(indexes);
    }
    else if (event->key() == Qt::Key_F5)
    {
        QSet<int> indexes;

        for (QModelIndex index: selectedIndexes())
            indexes.insert(index.row());

        emit F5Clicked(indexes);
    }
    else if (event->key() == Qt::Key_F6)
    {
        QSet<int> indexes;

        for (QModelIndex index: selectedIndexes())
            indexes.insert(index.row());

        emit F6Clicked(indexes);
    }
    else if (event->key() == Qt::Key_F7)
    {
        emit F7Clicked();
    }
    else if (event->key() == Qt::Key_Tab)
    {
        emit tabClicked();
    }
    else
        QTableView::keyPressEvent(event);
}
