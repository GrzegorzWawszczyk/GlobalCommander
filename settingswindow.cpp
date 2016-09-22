#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QDebug>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    QSettings settings;
    settings.beginGroup("FilesView");

    QFont font = QFont("Segoe UI", 9, QFont::Bold);
    ui->cb_headerFontFamily->setCurrentFont(settings.value("headerFont", font).value<QFont>());
    ui->cb_headerFontSize->setCurrentText(QString::number(settings.value("headerFont", font).value<QFont>().pointSize()));
    ui->rb_headerBold->setChecked(settings.value("headerFont", font).value<QFont>().bold());

    font = QFont("MS Shell Dlg 2", 7, QFont::Normal);
    ui->cb_fileListFontFamily->setCurrentFont(settings.value("fileListFont", font).value<QFont>());
    ui->cb_fileListFontSize->setCurrentText(QString::number(settings.value("fileListFont", font).value<QFont>().pointSize()));
    ui->rb_fileListBold->setChecked(settings.value("fileListFont", font).value<QFont>().bold());

    settings.endGroup();

    connect(ui->pb_apply, &QPushButton::clicked, this, &SettingsWindow::saveSettings);

}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

QFont SettingsWindow::getHeaderFont()
{
    QSettings settings;
    settings.beginGroup("FilesView");
    QFont headerFont = settings.value("headerFont", QFont("Segoe UI", 9, QFont::Bold)).value<QFont>();
    settings.endGroup();
    return headerFont;
}

void SettingsWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("FilesView");

    int bold = ui->rb_headerBold->isChecked() ? QFont::Bold : QFont::Normal;
    QFont font = QFont(ui->cb_headerFontFamily->currentText(), ui->cb_headerFontSize->currentText().toInt(), bold);
    settings.setValue("headerFont", font);

    bold = ui->rb_fileListBold->isChecked() ? QFont::Bold : QFont::Normal;
    font = QFont(ui->cb_fileListFontFamily->currentText(), ui->cb_fileListFontSize->currentText().toInt(), bold);
    settings.setValue("fileListFont", font);

    settings.endGroup();

}
