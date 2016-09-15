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
        emit deleteClicked(currentIndex().row());
    else
        QTableView::keyPressEvent(event);
}
