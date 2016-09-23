#include "filelistmodel.h"
#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QBitArray>
#include <QDebug>
#include <QFontDialog>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    //FONT SETTINGS
    headerFont = getHeaderFont();
    fileListFont = getFileListFont();

    setFontInfo();

    connect(ui->pb_changeHeaderFont, &QPushButton::clicked, this, [this](){
       headerFont = QFontDialog::getFont(nullptr, headerFont,this);
       if (headerFont.pointSize() > 20)
           headerFont.setPointSize(20);
       setFontInfo();
    });

    connect(ui->pb_changeFileListFont, &QPushButton::clicked, this, [this](){
        fileListFont = QFontDialog::getFont(nullptr, fileListFont,this);
        if (fileListFont.pointSize() > 20)
            fileListFont.setPointSize(20);
        setFontInfo();
    });

    //COLUMNS SETTINGS
    enabledColumns = SettingsWindow::getEnabledColumns();
    ui->chb_icon->setChecked(enabledColumns.at(FileListModel::Columns::Icon));
    ui->chb_extension->setChecked(enabledColumns.at(FileListModel::Columns::Extension));
    ui->chb_size->setChecked(enabledColumns.at(FileListModel::Columns::Size));
    ui->chb_date->setChecked(enabledColumns.at(FileListModel::Columns::Date));
    ui->chb_attributes->setChecked(enabledColumns.at(FileListModel::Columns::Attributes));



    connect(ui->pb_apply, &QPushButton::clicked, this, &SettingsWindow::saveSettings);
    connect(ui->pb_ok, &QPushButton::clicked, this, [this](){
        saveSettings();
        this->close();
    });
    connect(ui->pb_cancel, &QPushButton::clicked, this, &SettingsWindow::close);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

QFont SettingsWindow::getHeaderFont()
{
    QSettings settings;
    settings.beginGroup("FilesView");
    QFont headerFontSetting = settings.value("headerFont", QFont("Segoe UI", 9, QFont::Bold)).value<QFont>();
    settings.endGroup();
    return headerFontSetting;
}

QFont SettingsWindow::getFileListFont()
{
    QSettings settings;
    settings.beginGroup("FilesView");
    QFont fileListFontSetting = settings.value("fileListFont", QFont("Segoe UI", 7, QFont::Normal)).value<QFont>();
    settings.endGroup();
    return fileListFontSetting;
}

QBitArray SettingsWindow::getEnabledColumns()
{
    QBitArray enabledColumnsSetting(FileListModel::Count);

    QSettings settings;
    settings.beginGroup("FilesView");

    enabledColumnsSetting.setBit(FileListModel::Icon, settings.value("iconColumnEnabled", true).toBool());
    enabledColumnsSetting.setBit(FileListModel::Extension, settings.value("extensionColumnEnabled", true).toBool());
    enabledColumnsSetting.setBit(FileListModel::Size, settings.value("sizeColumnEnabled", true).toBool());
    enabledColumnsSetting.setBit(FileListModel::Date, settings.value("dateColumnEnabled", true).toBool());
    enabledColumnsSetting.setBit(FileListModel::Attributes, settings.value("attributesColumnEnabled", true).toBool());

    settings.endGroup();

    return enabledColumnsSetting;
}

void SettingsWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("FilesView");

    settings.setValue("headerFont", headerFont);
    settings.setValue("fileListFont", fileListFont);

    settings.setValue("iconColumnEnabled", ui->chb_icon->isChecked());
    settings.setValue("extensionColumnEnabled",ui->chb_extension->isChecked());
    settings.setValue("sizeColumnEnabled", ui->chb_size->isChecked());
    settings.setValue("dateColumnEnabled", ui->chb_date->isChecked());
    settings.setValue("attributesColumnEnabled", ui->chb_attributes->isChecked());

    settings.endGroup();

    emit settingsChanged();
}

void SettingsWindow::setFontInfo()
{
    ui->l_headerFontPreview->setFont(headerFont);
    ui->l_fileListFontPreview->setFont(fileListFont);

    ui->l_headerFont->setText(headerFont.family() + ", " + QString::number(headerFont.pointSize()) + (headerFont.bold() ? ", b" : ""));
    ui->l_fileListFont->setText(fileListFont.family() + ", " + QString::number(fileListFont.pointSize()) + (fileListFont.bold() ? ", b" : ""));
}
