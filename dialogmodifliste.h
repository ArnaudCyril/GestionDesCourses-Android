#ifndef DIALOGMODIFLISTE_H
#define DIALOGMODIFLISTE_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class DialogModifLIste;
}

class DialogModifLIste : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifLIste(QWidget *parent = 0,QString Fav="Supprimer");
    ~DialogModifLIste();
    QString id1;
     QString id2;
    QString table;
    QString champ1;
    QString champ2;

private slots:
    void on_pushButtonAnnulerMOdifListe_clicked();

    void on_pushButtonSupprLIste_clicked();

    void on_pushButtonModifLIste_clicked();

private:
    Ui::DialogModifLIste *ui;
};

#endif // DIALOGMODIFLISTE_H
