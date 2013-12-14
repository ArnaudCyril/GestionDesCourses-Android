#ifndef RAYON_H
#define RAYON_H

#include <QDialog>

namespace Ui {
class Rayon;
}

class Rayon : public QDialog
{
    Q_OBJECT

public:
    explicit Rayon(QWidget *parent = 0);
    ~Rayon();

private:
    Ui::Rayon *ui;
};

#endif // RAYON_H
