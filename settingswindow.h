#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QSettings>
#include <QWidget>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

    static QFont getHeaderFont();

private slots:
    void saveSettings();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
