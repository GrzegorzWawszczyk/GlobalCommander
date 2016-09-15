#include "drivescombobox.h"

#include <QDebug>
#include <QFileIconProvider>

DrivesComboBox::DrivesComboBox(QWidget *parent) : QComboBox(parent)
{

}

void DrivesComboBox::showPopup()
{
    QString lastItem = currentText();
    emit clicked();
    QComboBox::showPopup();
    setCurrentText(lastItem);
}
