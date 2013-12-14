#ifndef ORDRERAYON_H
#define ORDRERAYON_H

#include <QDialog>

namespace Ui {
class OrdreRayon;
}

class OrdreRayon : public QDialog
{
    Q_OBJECT

public:
    explicit OrdreRayon(QWidget *parent = 0);
    ~OrdreRayon();
    void chargerRayon();

private slots:
    void on_pushButtonSauverOrdre_clicked();

    void on_pushButtonAnnulerOrdre_clicked();

private:
    Ui::OrdreRayon *ui;
};

#endif // ORDRERAYON_H
