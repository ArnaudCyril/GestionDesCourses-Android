#include "dialogqtenewprod.h"
#include "ui_dialogqtenewprod.h"

DialogQteNewProd::DialogQteNewProd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogQteNewProd)
{
    ui->setupUi(this);
     ui->pushButton_2->setEnabled(false);



}

DialogQteNewProd::~DialogQteNewProd()
{
    delete ui;
}
void DialogQteNewProd::getProd(QString prod, QString lis)
{
    produit=prod;
    liste=lis;
    ui->label_2->setText(((MainWindow *)parent())->qteNewProd);
    if(((MainWindow *)parent())->qteNewProd.toInt()>1)
     {
        ui->pushButton_2->setEnabled(true);
    }
}

void DialogQteNewProd::on_pushButton_4_clicked()
{
    this->close();
    ((MainWindow *)parent())->qteNewProd="1";

}

void DialogQteNewProd::on_pushButton_3_clicked()
{
    if(((MainWindow *)parent())->newProd)
    {
        qDebug()<<"insert into prodInList values('"+produit+"' , '"+liste+"' , 'false' , "+ui->label_2->text()+") ";
    QSqlQuery req("insert into prodInList values('"+produit+"' , '"+liste+"' , 'false' , "+ui->label_2->text()+") ");
    }
    else
    {
        qDebug()<<"update prodInList set qte="+ui->label_2->text()+" where idProd="+((MainWindow *)parent())->idNewProd+" and idListe= "+((MainWindow *)parent())->maListe;
        QSqlQuery req("update prodInList set qte="+ui->label_2->text()+" where idProd="+((MainWindow *)parent())->idNewProd+" and idListe= "+((MainWindow *)parent())->maListe);
    }
    ((MainWindow *)parent())->chargerAjoutProduit();
    ((MainWindow *)parent())->chargerFavoris();
    ((MainWindow *)parent())->qteNewProd="1";
    this->close();
}
void DialogQteNewProd::on_pushButton_clicked()
{
    ui->label_2->setText(QString::number(ui->label_2->text().toInt()+1));
    if(ui->label_2->text().toInt()>1)
    {
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
    }
}
void DialogQteNewProd::on_pushButton_2_clicked()
{
    ui->label_2->setText(QString::number(ui->label_2->text().toInt()-1));
    if(ui->label_2->text().toInt()>1)
    {
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
    }

}
