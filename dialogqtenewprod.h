#ifndef DIALOGQTENEWPROD_H
#define DIALOGQTENEWPROD_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include "mainwindow.h"
namespace Ui {
class DialogQteNewProd;
}

class DialogQteNewProd : public QDialog
{
    Q_OBJECT

public:
    explicit DialogQteNewProd(QWidget *parent = 0);
    ~DialogQteNewProd();
    void getProd(QString,QString);
    QString liste,produit;
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogQteNewProd *ui;
};

#endif // DIALOGQTENEWPROD_H




