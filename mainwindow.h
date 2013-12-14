#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QVector>
#include <QModelIndex>
#include <QListWidgetItem>
#include <QTimer>
#include "dialogmodifliste.h"
#include "dialogqtenewprod.h"
#include "test.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase base;
    QVector <QString> vecteurListes;
    QVector <QString> vecteurRayons;
    QVector <QString> vecteurAjoutProd;
    QVector <QString> vecteuProdInListe;
    QVector <QString> vecteuFavoris;
    QVector <QString> vecteuOrdreRayon;

    void ouvreBase();
    void verifBase();
    void chargerListe();
    void chargerMagasin();
    void chargerListeProduits();
    void chargerAjoutProduit();
    void chargerRayon();
    void fermeBase();
    void creaNewListe(QString);
    void gererModif();
    void chargerFavoris();
    void chargerOrdreRayon();
    void actualiserInfo(int,int);
    QString getPath(int arg);
    QString getPath2(int arg);
    QString getPath3(int arg);


    QString maListe;
    QString currentItem;
    QString rayonChoisi;
    QString currentRayonAffiche;
    QString idNewProd;
    QString qteNewProd;
    int compteur;
    bool newProd;
    QString AjoutIsFav;
    QString AjoutMagRayTable;
    QString onModifQuoi;


protected:

    void changeEvent(QEvent *e);

private slots:


    void afficheOptionProduit();
    void afficheOptionListe();
    void afficheOptionRayon();
    void afficheOptionMagasin();
  void afficheOptionProduitInBdd();
   void afficheOptionProduitFavoris();
    void on_pushButton_clicked();

    void on_stackedWidgetAppli_currentChanged(int arg1);


    void on_listWidgetMagasin_itemPressed(QListWidgetItem *item);



    void on_listWidgetListes_itemPressed(QListWidgetItem *item);



    void on_listWidgetListes_itemClicked(QListWidgetItem *item);


    void on_pushButtonProduitsListe_clicked();

    void on_listWidgetChoixRayon_itemClicked(QListWidgetItem *item);

    void on_pushButtonBack2_clicked();

    void on_pushButtonBack3_clicked();

    void on_pushButtonBack4_clicked();

    void on_pushButtonBack5_clicked();

    void on_listWidgetAjoutProduit_itemClicked(QListWidgetItem *item);

    void on_listWidgetProduitsListe_itemClicked(QListWidgetItem *item);

    void on_pushButtonProduitsListe_2_clicked();

    void on_pushButtonProduitsListe_3_clicked();

    void on_pushButtonLIste_3_clicked();

    void on_pushButtonLIste_2_clicked();

    void on_pushButtonLIste_clicked();



    void on_listWidgetProduitsListe_itemPressed(QListWidgetItem *item);

    void on_pushButtonAjoutProddBdd_clicked();

    void on_pushButtonAjoutProduit_clicked();

    void on_lineEditAjoutProd_cursorPositionChanged(int arg1, int arg2);

    void on_pushButtonAjoutProduit2_clicked();

    void on_pushButtonAnnulerAjoutProd_clicked();

    void on_pushButtonChangerFav_clicked();

    void on_listWidgetChoixRayon_itemPressed(QListWidgetItem *item);

    void on_listWidgetMagasin_itemClicked(QListWidgetItem *item);

    void on_listWidgetAjoutProduit_itemPressed(QListWidgetItem *item);

    void on_pushButtonAjouterMagasin_clicked();

    void on_pushButtonAjouterRayMag_clicked();

    void on_lineEditAjoutMagRay_cursorPositionChanged(int arg1, int arg2);

    void on_pushButtonAnnulerAjoutMagRay_clicked();

    void on_pushButtonAjoutRayon_clicked();

    void on_pushButtonModifierAnnulerAll_clicked();

    void on_pushButtonModifierAll_clicked();

    void on_pushButtonModifierQtePlus_clicked();

    void on_pushButtonModifierQTeMoins_clicked();

    void on_pushButtonChangerFavModifier_clicked();

    void on_pushButtonFavorisListe_clicked();

    void on_pushButtonFavorisListe_2_clicked();

    void on_pushButtonFavorisListe_3_clicked();

    void on_pushButtonLIste_9_clicked();

    void on_pushButtonProduitsListe_9_clicked();

    void on_pushButtonFavorisListe_9_clicked();

    void on_listWidgetFavoris_itemPressed(QListWidgetItem *item);

    void on_listWidgetFavoris_itemClicked(QListWidgetItem *item);

    void on_pushButtonOrdreRayon_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

private:

    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *timer2;
    QTimer *timer3;
    QTimer *timer4;
    QTimer *timer5;
    QTimer *timer6;


};

#endif // MAINWINDOW_H
