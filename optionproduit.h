#ifndef OPTIONPRODUIT_H
#define OPTIONPRODUIT_H

#include <QDialog>

namespace Ui {
class OptionProduit;
}

class OptionProduit : public QDialog
{
    Q_OBJECT

public:
    explicit OptionProduit(QWidget *parent = 0);
    ~OptionProduit();
    void getIdProduit(QString,QString);
    QString liste,prod;

private slots:
    void on_pushButtonAnnulerMOdifProd_clicked();

    void on_pushButtonSupprProd_clicked();

private:
    Ui::OptionProduit *ui;
};

#endif // OPTIONPRODUIT_H
