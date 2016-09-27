#include "filestableview.h"
#include "gcmdmainwindow.h"

#include <QAction>
#include <QApplication>
#include <QDebug>

FilesTableView::FilesTableView(QWidget *parent)
    : QTableView(parent)
{

}

void FilesTableView::copySelected()
{
    //Checking if this is focused view
    if (this == QApplication::focusWidget())
    {
        emit copyClicked(getSelectedIndexes());
    }
}

void FilesTableView::moveSelected()
{
    if (this == QApplication::focusWidget())
    {
        emit moveClicked(getSelectedIndexes());
    }
}

void FilesTableView::deleteSelected()
{
    if (this == QApplication::focusWidget())
    {
        emit deleteClicked(getSelectedIndexes());
    }
}

void FilesTableView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_T)
        emit newTabCombinationClicked();
    else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W)
        emit closeTabCombinationClicked();
    else if (event->key() == Qt::Key_Backspace)
        emit backspaceClicked();
    else if (event->key() == Qt::Key_Delete)
    {
        emit deleteClicked(getSelectedIndexes());
    }
    else if (event->key() == Qt::Key_F3)
    {
        emit F3Clicked(getSelectedIndexes());
    }
    else if (event->key() == Qt::Key_F5)
    {
        emit F5Clicked(getSelectedIndexes());
    }
    else if (event->key() == Qt::Key_F6)
    {
        emit F6Clicked(getSelectedIndexes());
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

QSet<int> FilesTableView::getSelectedIndexes()
{
    QSet<int> indexes;

    for (QModelIndex index: selectedIndexes())
        indexes.insert(index.row());

    return indexes;
}
