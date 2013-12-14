#include "ordrerayon.h"
#include "ui_ordrerayon.h"
#include <QSqlQuery>

OrdreRayon::OrdreRayon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrdreRayon)
{
    ui->setupUi(this);

    this->chargerRayon();
}

OrdreRayon::~OrdreRayon()
{
    delete ui;
}
void OrdreRayon::chargerRayon()
{

    QSqlQuery req("select libelle from rayon");

       while(req.next())
       {

           ui->listWidgetOrdreRayon->addItem(req.value(0).toString());

       }
}

void OrdreRayon::on_pushButtonSauverOrdre_clicked()
{
    this->close();
}

void OrdreRayon::on_pushButtonAnnulerOrdre_clicked()
{
    this->close();
}
