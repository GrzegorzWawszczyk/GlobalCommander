#include "drivelistmodel.h"
#include "filelistmodel.h"
#include "filestableview.h"
#include "gcmdmainwindow.h"
#include "settingswindow.h"
#include "ui_gcmdmainwindow.h"

#include <QComboBox>
#include <QDebug>
#include <QFileInfoList>
#include <QStorageInfo>

GCMDMainWindow::GCMDMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GCMDMainWindow)
{
    ui->setupUi(this);

    ui->tabW_left->clear();
    ui->tabW_right->clear();

    delete ui->to_remove;
    delete ui->to_remove2;
    delete ui->to_remove3;
    delete ui->to_remove4;

    ui->tabW_left->addTab(new FilesView(), QDir::drives().first().absolutePath());
    ui->tabW_right->addTab(new FilesView(), QDir::drives().first().absolutePath());

    FilesView* left = dynamic_cast<FilesView*>(ui->tabW_left->currentWidget());
    left->setAttribute(Qt::WA_DeleteOnClose, true);
    FilesView* right = dynamic_cast<FilesView*>(ui->tabW_right->currentWidget());
    right->setAttribute(Qt::WA_DeleteOnClose, true);

    left->setSecondView(right);
    right->setSecondView(left);

    ui->tabW_left->addTab(left, QDir::drives().first().absolutePath());
    ui->tabW_right->addTab(right, QDir::drives().first().absolutePath());

    ui->tabW_left->currentWidget()->setFocus();


    connect(ui->a_copy, &QAction::triggered, left, &FilesView::handleActionCopyClick);
    connect(ui->a_copy, &QAction::triggered, right, &FilesView::handleActionCopyClick);
    connect(ui->pb_copy, &QPushButton::clicked, left, &FilesView::handleActionCopyClick);
    connect(ui->pb_copy, &QPushButton::clicked, right, &FilesView::handleActionCopyClick);

    connect(ui->a_move, &QAction::triggered, left, &FilesView::handleActionMoveClick);
    connect(ui->a_move, &QAction::triggered, right, &FilesView::handleActionMoveClick);
    connect(ui->pb_move, &QPushButton::clicked, left, &FilesView::handleActionMoveClick);
    connect(ui->pb_move, &QPushButton::clicked, right, &FilesView::handleActionMoveClick);

    connect(ui->a_delete, &QAction::triggered, left, &FilesView::handleActionDeleteClick);
    connect(ui->a_delete, &QAction::triggered, right, &FilesView::handleActionDeleteClick);
    connect(ui->pb_delete, &QPushButton::clicked, left, &FilesView::handleActionDeleteClick);
    connect(ui->pb_delete, &QPushButton::clicked, right, &FilesView::handleActionDeleteClick);

    connect(ui->a_createDir, &QAction::triggered, left, &FilesView::handleActionCreateDirClick);
    connect(ui->a_createDir, &QAction::triggered, right, &FilesView::handleActionCreateDirClick);
    connect(ui->pb_mkDir, &QPushButton::clicked, left, &FilesView::handleActionCreateDirClick);
    connect(ui->pb_mkDir, &QPushButton::clicked, right, &FilesView::handleActionCreateDirClick);

    connect(left, &FilesView::newTabCombinationClicked, this, &GCMDMainWindow::addTabLeft);
    connect(left, &FilesView::closeTabCombinationClicked, this, [this]()
    {
        if (ui->tabW_left->count() > 1)
        {
            QWidget* toDelete = ui->tabW_left->currentWidget();
            ui->tabW_left->removeTab(ui->tabW_left->currentIndex());
            delete toDelete;
        }
    });
    connect(right, &FilesView::newTabCombinationClicked, this, &GCMDMainWindow::addTabRight);
    connect(right, &FilesView::closeTabCombinationClicked, this, [this]()
    {
        if (ui->tabW_right->count() > 1)
        {
            QWidget* toDelete = ui->tabW_right->currentWidget();
            ui->tabW_right->removeTab(ui->tabW_right->currentIndex());
            delete toDelete;
        }
    });

    connect(left, &FilesView::pathChanged, this, &GCMDMainWindow::changeLeftTabText);
    connect(right, &FilesView::pathChanged, this, &GCMDMainWindow::changeRightTabText);

    connect(left, &FilesView::deletedDirectory, this, &GCMDMainWindow::checkIfRightViewsInDeletedDirectory);
    connect(right, &FilesView::deletedDirectory, this, &GCMDMainWindow::checkIfLeftViewsInDeletedDirectory);

    connect(ui->tabW_left, &QTabWidget::currentChanged, this, [this](){
        dynamic_cast<FilesView*>(ui->tabW_left->currentWidget())->setSecondView(dynamic_cast<FilesView*>(ui->tabW_right->currentWidget()));
        dynamic_cast<FilesView*>(ui->tabW_right->currentWidget())->setSecondView(dynamic_cast<FilesView*>(ui->tabW_left->currentWidget()));
        ui->tabW_left->currentWidget()->setFocus();
    });

    connect(ui->tabW_right, &QTabWidget::currentChanged, this, [this](){
        dynamic_cast<FilesView*>(ui->tabW_left->currentWidget())->setSecondView(dynamic_cast<FilesView*>(ui->tabW_right->currentWidget()));
        dynamic_cast<FilesView*>(ui->tabW_right->currentWidget())->setSecondView(dynamic_cast<FilesView*>(ui->tabW_left->currentWidget()));
        ui->tabW_right->currentWidget()->setFocus();
    });

    connect(ui->tabW_left, &QTabWidget::tabCloseRequested, this, [this](int index){
        QWidget* toDelete = ui->tabW_left->widget(index);
        ui->tabW_left->removeTab(index);
        delete toDelete;
    });

    connect(ui->tabW_right, &QTabWidget::tabCloseRequested, this, [this](int index){
        QWidget* toDelete = ui->tabW_right->widget(index);
        ui->tabW_right->removeTab(index);
        delete toDelete;
    });

    connect(ui->a_settings, &QAction::triggered, this, [this](){
       SettingsWindow* settingsWindow = new SettingsWindow();
       settingsWindow->setAttribute(Qt::WA_DeleteOnClose, true);
       settingsWindow->show();
       connect(settingsWindow, &SettingsWindow::settingsChanged, this, [this](){
          for (int widgetIndex = 0; widgetIndex < ui->tabW_left->count(); widgetIndex++)
              dynamic_cast<FilesView*>(ui->tabW_left->widget(widgetIndex))->refreshView();

          for (int widgetIndex = 0; widgetIndex < ui->tabW_right->count(); widgetIndex++)
              dynamic_cast<FilesView*>(ui->tabW_right->widget(widgetIndex))->refreshView();
       });
    });

}

GCMDMainWindow::~GCMDMainWindow()
{
    delete ui;
}

void GCMDMainWindow::addTabLeft(const QDir &path)
{
    FilesView* filesView = new FilesView();
    filesView->setAttribute(Qt::WA_DeleteOnClose, true);
    filesView->setDirectory(path.path());
    filesView->setDrive(path.path());
    filesView->setSecondView(dynamic_cast<FilesView*>(ui->tabW_right->currentWidget()));
    dynamic_cast<FilesView*>(ui->tabW_right->currentWidget())->setSecondView(filesView);

    connect(ui->a_copy, &QAction::triggered, filesView, &FilesView::handleActionCopyClick);
    connect(ui->a_move, &QAction::triggered, filesView, &FilesView::handleActionMoveClick);
    connect(ui->a_delete, &QAction::triggered, filesView, &FilesView::handleActionDeleteClick);
    connect(ui->a_createDir, &QAction::triggered, filesView, &FilesView::handleActionCreateDirClick);
    connect(ui->pb_copy, &QPushButton::clicked, filesView, &FilesView::handleActionCopyClick);
    connect(ui->pb_move, &QPushButton::clicked, filesView, &FilesView::handleActionMoveClick);
    connect(ui->pb_delete, &QPushButton::clicked, filesView, &FilesView::handleActionDeleteClick);
    connect(ui->pb_mkDir, &QPushButton::clicked, filesView, &FilesView::handleActionCreateDirClick);
    connect(filesView, &FilesView::newTabCombinationClicked, this, &GCMDMainWindow::addTabLeft);
    connect(filesView, &FilesView::closeTabCombinationClicked, this, [this](){
        if (ui->tabW_left->count() > 1)
        {
            QWidget* toDelete = ui->tabW_left->currentWidget();
            ui->tabW_left->removeTab(ui->tabW_left->currentIndex());
            delete toDelete;
        }
    });
    connect(filesView, &FilesView::pathChanged, this, &GCMDMainWindow::changeLeftTabText);

    if (!path.isRoot())
        ui->tabW_left->addTab(filesView, path.dirName());
    else
        ui->tabW_left->addTab(filesView, path.path());

    connect(filesView, &FilesView::deletedDirectory, this, &GCMDMainWindow::checkIfRightViewsInDeletedDirectory);
}

void GCMDMainWindow::addTabRight(const QDir &path)
{
    FilesView* filesView = new FilesView();
    filesView->setAttribute(Qt::WA_DeleteOnClose, true);
    filesView->setDirectory(path.path());
    filesView->setDrive(path.path());
    filesView->setSecondView(dynamic_cast<FilesView*>(ui->tabW_left->currentWidget()));
    dynamic_cast<FilesView*>(ui->tabW_left->currentWidget())->setSecondView(filesView);

    connect(ui->a_copy, &QAction::triggered, filesView, &FilesView::handleActionCopyClick);
    connect(ui->pb_copy, &QPushButton::clicked, filesView, &FilesView::handleActionCopyClick);
    connect(ui->a_move, &QAction::triggered, filesView, &FilesView::handleActionMoveClick);
    connect(ui->a_delete, &QAction::triggered, filesView, &FilesView::handleActionDeleteClick);
    connect(ui->a_createDir, &QAction::triggered, filesView, &FilesView::handleActionCreateDirClick);
    connect(ui->pb_copy, &QPushButton::clicked, filesView, &FilesView::handleActionCopyClick);
    connect(ui->pb_move, &QPushButton::clicked, filesView, &FilesView::handleActionMoveClick);
    connect(ui->pb_delete, &QPushButton::clicked, filesView, &FilesView::handleActionDeleteClick);
    connect(ui->pb_mkDir, &QPushButton::clicked, filesView, &FilesView::handleActionCreateDirClick);
    connect(filesView, &FilesView::newTabCombinationClicked, this, &GCMDMainWindow::addTabRight);
    connect(filesView, &FilesView::closeTabCombinationClicked, this, [this]()
    {
        if (ui->tabW_right->count() > 1)
        {
            QWidget* toDelete = ui->tabW_right->currentWidget();
            ui->tabW_right->removeTab(ui->tabW_right->currentIndex());
            delete toDelete;
        }
    });
    connect(filesView, &FilesView::pathChanged, this, &GCMDMainWindow::changeRightTabText);

    if (!path.isRoot())
        ui->tabW_right->addTab(filesView, path.dirName());
    else
        ui->tabW_right->addTab(filesView, path.path());

    connect(filesView, &FilesView::deletedDirectory, this, &GCMDMainWindow::checkIfLeftViewsInDeletedDirectory);
}

void GCMDMainWindow::changeLeftTabText(const QString &pathName, FilesView *exactView) //Couldn't use const for pointer
{
    ui->tabW_left->setTabText(ui->tabW_left->indexOf(exactView), pathName);
}

void GCMDMainWindow::changeRightTabText(const QString &pathName, FilesView *exactView) //Couldn't use const for pointer
{
    ui->tabW_right->setTabText(ui->tabW_right->indexOf(exactView), pathName);
}

void GCMDMainWindow::checkIfLeftViewsInDeletedDirectory(const QString &path)
{
    for (int widgetIndex = 0; widgetIndex < ui->tabW_left->count(); widgetIndex++)
        dynamic_cast<FilesView*>(ui->tabW_left->widget(widgetIndex))->checkIfInDeletedDirectory(path);
}

void GCMDMainWindow::checkIfRightViewsInDeletedDirectory(const QString &path)
{

    for (int widgetIndex = 0; widgetIndex < ui->tabW_right->count(); widgetIndex++)
        dynamic_cast<FilesView*>(ui->tabW_right->widget(widgetIndex))->checkIfInDeletedDirectory(path);
}
