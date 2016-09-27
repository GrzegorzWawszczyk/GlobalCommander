#ifndef LISTER_H
#define LISTER_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class Lister;
}

class Lister : public QDialog
{
    Q_OBJECT

public:
    explicit Lister(const QString &path, QWidget *parent = 0);
    ~Lister();

private:
    Ui::Lister *ui;
};

#endif // LISTER_H
