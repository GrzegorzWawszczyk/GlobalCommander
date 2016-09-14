#ifndef DRIVESCOMBOBOX_H
#define DRIVESCOMBOBOX_H

#include <QComboBox>
#include <QObject>
#include <QWidget>

class DrivesComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DrivesComboBox(QWidget *parent = 0);

    void showPopup() override;

signals:
    void clicked();

public slots:
};

#endif // DRIVESCOMBOBOX_H
