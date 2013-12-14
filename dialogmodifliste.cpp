#include "dialogmodifliste.h"
#include "ui_dialogmodifliste.h"
#include "mainwindow.h"

DialogModifLIste::DialogModifLIste(QWidget *parent,QString Fav) :
    QDialog(parent),
    ui(new Ui::DialogModifLIste)
{
    ui->setupUi(this);

        ui->pushButtonSupprLIste->setText(Fav);



}

DialogModifLIste::~DialogModifLIste()
{
    delete ui;
}

void DialogModifLIste::on_pushButtonAnnulerMOdifListe_clicked()
{
    this->close();
}

void DialogModifLIste::on_pushButtonSupprLIste_clicked()
{

    if(id2=="nullos")
    {
        qDebug()<<"delete from prodInList where idListe="+id1;
        qDebug()<<"delete from "+table+" where "+champ1+"="+id1;

        QSqlQuery req("delete from prodInList where idLIste="+id1);
        QSqlQuery req3("delete from "+table+" where "+champ1+"="+id1);

    }
    else
    {
    if(id2=="nullatre")
    {
        QString temp;
        int i=0;
            QSqlQuery req6("select idMagasin from magasin where libelle="+id1);
            req6.first();
            temp=req6.value(0).toString();
            qDebug()<<"select idListe from liste where idMagasin="+temp;
             QSqlQuery req7("select idListe from liste inner  where idMagasin="+temp);
             while(req7.next())
             {
                 qDebug()<<"delete from prodInList where idListe="+req7.value(0).toString();
                 QSqlQuery req5("delete from prodInList where idListe="+req7.value(0).toString());
             }
        QSqlQuery req4("select idMagasin from magasin where libelle="+id1);
        while(req4.next())
        {
            i++;
           qDebug()<<"delete from liste where idMagasin="+req4.value(0).toString();
         QSqlQuery req2("delete from liste where idMagasin="+req4.value(0).toString());
        }

            qDebug()<<"delete from "+table+" where "+champ1+"="+id1;
        QSqlQuery req3("delete from "+table+" where "+champ1+"="+id1);


    }
    else
    {
    if(champ2=="favoris")
    {
        QSqlQuery req("update Produit set isFavoris='false' where idProd="+id1);
        ui->pushButtonSupprLIste->setText("Supprimer des Favoris");
    }
    else
    {
        ui->pushButtonSupprLIste->setText("Supprimer");

    if(champ2!="null")
    {
        QSqlQuery req("delete from "+table+" where "+champ1+"="+id1+" and "+champ2+"="+id2);
    }
    else
    {
        QSqlQuery req("delete from "+table+" where "+champ1+"="+id1);
    }
    if(table=="Produit")
    {
        QSqlQuery req("delete from prodInList where idProd="+id1+" and idListe="+((MainWindow *)parent())->maListe);
    }
    }
    }
    }
    ((MainWindow *)parent())->chargerListeProduits();
    ((MainWindow *)parent())->chargerRayon();
    ((MainWindow *)parent())->chargerMagasin();
    ((MainWindow *)parent())->chargerAjoutProduit();
    ((MainWindow *)parent())->chargerListe();
      ((MainWindow *)parent())->chargerFavoris();
    this->close();

}

void DialogModifLIste::on_pushButtonModifLIste_clicked()
{
    ((MainWindow *)parent())->gererModif();
    this->close();



}
