#include "drivelistmodel.h"
#include "filelistmodel.h"
#include "filestableview.h"
#include "gcmdmainwindow.h"
#include "ui_gcmdmainwindow.h"

#include <QComboBox>
#include <QDebug>
#include <QFileInfoList>
#include <QStorageInfo>
#include <filesview.h>

GCMDMainWindow::GCMDMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GCMDMainWindow)
{
    ui->setupUi(this);

    dynamic_cast<FilesView*>(ui->fv_left)->setSecondView(ui->fv_right);
    dynamic_cast<FilesView*>(ui->fv_right)->setSecondView(ui->fv_left);

    connect(ui->a_copy, &QAction::triggered, ui->fv_left, &FilesView::handleActionCopyClick);
    connect(ui->a_copy, &QAction::triggered, ui->fv_right, &FilesView::handleActionCopyClick);

    connect(ui->a_move, &QAction::triggered, ui->fv_left, &FilesView::handleActionMoveClick);
    connect(ui->a_move, &QAction::triggered, ui->fv_right, &FilesView::handleActionMoveClick);

    connect(ui->a_delete, &QAction::triggered, ui->fv_left, &FilesView::handleActionDeleteClick);
    connect(ui->a_delete, &QAction::triggered, ui->fv_right, &FilesView::handleActionDeleteClick);

//    connect(ui->a_copy, &QAction::triggered, dynamic_cast<FilesTableView*>(QApplication::focusWidget()), &FilesTableView::copySelected);
}

GCMDMainWindow::~GCMDMainWindow()
{
    delete ui;
}

QAction *GCMDMainWindow::getActionCopyPointer()
{
    return ui->a_copy;
}
