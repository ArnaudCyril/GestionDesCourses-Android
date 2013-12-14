#include "rayon.h"
#include "ui_rayon.h"

Rayon::Rayon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rayon)
{
    ui->setupUi(this);
}

Rayon::~Rayon()
{
    delete ui;
}
