#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QBitArray>
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
    static QFont getFileListFont();
    static QBitArray getEnabledColumns();

signals:
    void settingsChanged();


private slots:
    void saveSettings();

private:
    Ui::SettingsWindow *ui;
    QFont headerFont;
    QFont fileListFont;
    QBitArray enabledColumns;
    void setFontInfo();

};

#endif // SETTINGSWINDOW_H
