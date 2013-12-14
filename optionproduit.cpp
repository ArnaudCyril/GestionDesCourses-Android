#include "optionproduit.h"
#include "ui_optionproduit.h"
#include "mainwindow.h"
OptionProduit::OptionProduit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionProduit)
{
    ui->setupUi(this);
}

OptionProduit::~OptionProduit()
{
    delete ui;
}

void OptionProduit::getIdProduit(QString p, QString l)
{
    prod=p;liste=l;
}

void OptionProduit::on_pushButtonAnnulerMOdifProd_clicked()
{
    this->close();
}

void OptionProduit::on_pushButtonSupprProd_clicked()
{
    qDebug()<<"delete from prodInList where idListe="+liste+" and idProd="+prod;
    QSqlQuery req2("delete from prodInList where idListe="+liste+" and idProd="+prod);


    ((MainWindow *)parent())->chargerListeProduits();
    this->close();
}
