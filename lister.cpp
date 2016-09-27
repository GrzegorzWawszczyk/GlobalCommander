#include "lister.h"
#include "ui_lister.h"

#include <QFile>
#include <QTextStream>


Lister::Lister(const QString &path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lister)
{
    ui->setupUi(this);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        this->close();

    QTextStream in(&file);

    ui->textBrowser->setText(in.readAll());
}

Lister::~Lister()
{
    delete ui;
}
