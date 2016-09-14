#include "drivescombobox.h"

#include <QDebug>

DrivesComboBox::DrivesComboBox(QWidget *parent) : QComboBox(parent)
{

}

void DrivesComboBox::showPopup()
{
    QString lastItem = this->currentText();
    emit clicked();
    QComboBox::showPopup();
    this->setCurrentIndex(this->findText(lastItem));
}
