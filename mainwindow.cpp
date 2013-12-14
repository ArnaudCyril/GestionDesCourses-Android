#include "mainwindow.h"
#include "ui_mainwindow.h"
 #include <QDesktopWidget>
#include <QDrag>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QTableWidgetItem>
#include <ordrerayon.h>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QIcon>
#include <QListWidget>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     ui->centralWidget->showMaximized(); // met la taille de la ppli a fond
    this->ouvreBase();
     setMouseTracking(true);
    ui->listWidgetChoixRayon->setMouseTracking(true);
     chargerListe();
     timer = new QTimer(this);
     timer->setSingleShot(true);
     connect(timer, SIGNAL(timeout()), this, SLOT(afficheOptionListe()));
     timer2 = new QTimer(this);
     timer2->setSingleShot(true);
     connect(timer2, SIGNAL(timeout()), this, SLOT(afficheOptionProduit()));
     timer3 = new QTimer(this);
     timer3->setSingleShot(true);
     connect(timer3, SIGNAL(timeout()), this, SLOT(afficheOptionRayon()));
     timer4 = new QTimer(this);
     timer4->setSingleShot(true);
    connect(timer4, SIGNAL(timeout()), this, SLOT(afficheOptionMagasin()));

    timer5 = new QTimer(this);
     timer5->setSingleShot(true);
    connect(timer5, SIGNAL(timeout()), this, SLOT(afficheOptionProduitInBdd()));
    timer6 = new QTimer(this);
     timer6->setSingleShot(true);
    connect(timer6, SIGNAL(timeout()), this, SLOT(afficheOptionProduitFavoris()));


      AjoutIsFav="false";
      compteur=0;
      ui->pushButtonModifierQtePlus->setVisible(false);
      ui->pushButtonModifierQTeMoins->setVisible(false);
      ui->pushButtonChangerFavModifier->setVisible(false);
      ui->labelIsFavModifier->setVisible(false);
      qteNewProd="1";
    ui->stackedWidgetAppli->setCurrentIndex(0);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::afficheOptionProduit()
{
    QSqlQuery req("select libelle from Rayon");
    while(req.next())
    {
        if(currentItem==req.value(0).toString() or currentItem==req.value(0).toString()+" Rayon Complet")
        {
            currentItem="rayonOnAffichePas";
        }
    }

    if(currentItem==ui->listWidgetProduitsListe->currentItem()->text())
    {
    DialogModifLIste OptionListe(this);
        //on fait passer les informations
        //on veut placer la fenetre correctement
    OptionListe.table="prodInList";
    OptionListe.champ1="idListe";
    OptionListe.id1=maListe;
    OptionListe.champ2="idProd";
    OptionListe.id2=vecteuProdInListe.value(ui->listWidgetProduitsListe->currentRow());
            QDesktopWidget *desktop = QApplication::desktop();
            int screenWidth, width;
            int screenHeight, height;
            int x, y;
            QSize windowSize;
            screenWidth = desktop->width(); //     obtenir la largeur de l'écran
            screenHeight = desktop->height(); // obtenir la hauteur de l'écran

           //peu de calculs
            x = screenWidth /4 ;
            y = screenHeight / 2;
            y -= 100;

            OptionListe.move( x, y );// déplace la fenêtre de coordonnées


        OptionListe.exec();
      }

}


void MainWindow::ouvreBase()
{
    base=QSqlDatabase::addDatabase("QSQLITE");
    //nom fichier qui contient la base sqlite3
    base.setDatabaseName("listeCourses.db");

    //ouverture de la base de données
    base.open();

    //si la base ne s'est pas ouverte correctement
    if(base.isOpen() != true){
            QMessageBox::critical(this, "Erreur critique", "Impossible d'ouvrir la base de données.");
        }
    else{
            this->verifBase();
    }
}
void MainWindow::verifBase()
{
    this->base.exec("select id,libelle,quantite,idRayon, isBought,isTemp from produit;");
    this->base.exec("select idRayon,libelle,ordre from rayon;");



    if (this->base.lastError().number()>0)//si la requete renvoie une erreur
    {
       this->base.exec("begin;"); //début transaction SQL
        this->base.exec("PRAGMA foreign_keys = ON");
        this->base.exec("drop table if exists magasin");
        this->base.exec("create table magasin(idMagasin smallint primary key , libelle varchar(50))");
        this->base.exec("drop table if exists liste");
        this->base.exec("create table liste(idListe smallint primary key ,idMagasin varchar(50), libelle varchar(50) ,isFinished bool, FOREIGN KEY(idMagasin) REFERENCES magasin(idMagasin))");
        this->base.exec("drop table if exists prodInList;");
        this->base.exec("create table prodInList(idProd smallint , idListe smallint  , isBought bool , qte smallint , FOREIGN KEY(idProd) REFERENCES produit(idProd), FOREIGN KEY(idListe) REFERENCES liste(idListe))");
        this->base.exec("drop table if exists produit;");
        this->base.exec("create table produit(idProd smallint primary key ,libelle varchar(50), idRayon smallint , isFavoris bool, FOREIGN KEY(idRayon) REFERENCES rayon(idRayon));");
        this->base.exec("drop table if exists rayon");
        this->base.exec("create table rayon(idRayon smallint primary key ,libelle varchar(50),ordre tinyint)");

        // insertion des magasins
        this->base.exec("insert into magasin values(1,'Carrefour')");
        this->base.exec("insert into magasin values(2,'Leclerc')");
        this->base.exec("insert into magasin values(3,'Aldi')");
        this->base.exec("insert into magasin values(4,'Super U')");
        //insertion des rayons
        this->base.exec("insert into rayon values(1,'Produits Frais',1)");
        this->base.exec("insert into rayon values(2,'Fruits & Légumes',2)");
        this->base.exec("insert into rayon values(3,'Alimentaire',3)");
        this->base.exec("insert into rayon values(4,'Sucreries',4)");
        this->base.exec("insert into rayon values(5,'Surgelé',5)");
        this->base.exec("insert into rayon values(6,'Hygiene',6)");
        this->base.exec("insert into rayon values(7,'Animaux',7)");
        this->base.exec("insert into rayon values(8,'Vetement',8)");
        this->base.exec("insert into rayon values(9,'Autres',9)");
        this->base.exec("insert into rayon values(10,'Boissons',10)");
        this->base.exec("insert into rayon values(11,'Alcools',11)");
         this->base.exec("insert into rayon values(12,'Petit Dejeuner',12)");



        //insertion des produits
        this->base.exec("insert into produit values(1,'Bonbon',4,'false')");
        this->base.exec("insert into produit values(11,'Biscuit',4,'false')");
        this->base.exec("insert into produit values(12,'Chocolat',4,'false')");
        this->base.exec("insert into produit values(13,'Barres Choc',4,'false')");
        this->base.exec("insert into produit values(14,'Sucettes',4,'false')");
        this->base.exec("insert into produit values(15,'Twix',4,'false')");
        this->base.exec("insert into produit values(16,'Mars',4,'false')");
        this->base.exec("insert into produit values(17,'M&Ms',4,'false')");
        this->base.exec("insert into produit values(18,'Smarties',4,'false')");
        this->base.exec("insert into produit values(19,'Nougat',4,'false')");

        this->base.exec("insert into produit values(2,'Lait',1,'false')");
        this->base.exec("insert into produit values(21,'Fromage',1,'false')");
        this->base.exec("insert into produit values(22,'Oeufs',1,'false')");
        this->base.exec("insert into produit values(23,'Beurre',1,'false')");
        this->base.exec("insert into produit values(24,'Yahourt',1,'false')");
        this->base.exec("insert into produit values(25,'Danette',1,'false')");
        this->base.exec("insert into produit values(26,'Flan',1,'false')");
        this->base.exec("insert into produit values(27,'Actimel',1,'false')");
        this->base.exec("insert into produit values(28,'Pâte',1,'false')");
        this->base.exec("insert into produit values(29,'Creme Fraîche',1,'false')");

        this->base.exec("insert into produit values(3,'Savon',6,'false')");
        this->base.exec("insert into produit values(31,'Parfum',6,'false')");
        this->base.exec("insert into produit values(32,'Papier WC',6,'false')");
        this->base.exec("insert into produit values(33,'Gel Douche',6,'false')");
        this->base.exec("insert into produit values(34,'Brosse a Dent',6,'false')");
        this->base.exec("insert into produit values(35,'Mouchoirs',6,'false')");
        this->base.exec("insert into produit values(36,'Dentifrice',6,'false')");
        this->base.exec("insert into produit values(37,'Coton Tiges',6,'false')");
        this->base.exec("insert into produit values(38,'Rasoir',6,'false')");
        this->base.exec("insert into produit values(39,'Javel',6,'false')");

        this->base.exec("insert into produit values(4,'Tomates',2,'false')");
        this->base.exec("insert into produit values(41,'Carotes',2,'false')");
        this->base.exec("insert into produit values(42,'Courgettes',2,'false')");
        this->base.exec("insert into produit values(43,'Choux',2,'false')");
        this->base.exec("insert into produit values(44,'Aubergine',2,'false')");
        this->base.exec("insert into produit values(45,'Banane',2,'false')");
        this->base.exec("insert into produit values(46,'Abricot',2,'false')");
        this->base.exec("insert into produit values(47,'Pomme',2,'false')");
        this->base.exec("insert into produit values(48,'Poire',2,'false')");
        this->base.exec("insert into produit values(49,'Pêches',2,'false')");

        this->base.exec("insert into produit values(5,'Pates',3,'false')");
        this->base.exec("insert into produit values(51,'Pizza',3,'false')");
        this->base.exec("insert into produit values(52,'Lasagne',3,'false')");
        this->base.exec("insert into produit values(53,'Quiches',3,'false')");
        this->base.exec("insert into produit values(54,'Nems',3,'false')");
        this->base.exec("insert into produit values(55,'Sandwitchs',3,'false')");
        this->base.exec("insert into produit values(56,'Riz',3,'false')");
        this->base.exec("insert into produit values(57,'Taboulé',3,'false')");
        this->base.exec("insert into produit values(58,'Soupe',3,'false')");
        this->base.exec("insert into produit values(59,'Couscous',3,'false')");

        this->base.exec("insert into produit values(6,'Croquettes',7,'false')");
        this->base.exec("insert into produit values(61,'Litiere',7,'false')");
        this->base.exec("insert into produit values(62,'Pate',7,'false')");
        this->base.exec("insert into produit values(63,'Jeu ',7,'false')");
        this->base.exec("insert into produit values(64,'Nourr Poisson',7,'false')");
        this->base.exec("insert into produit values(65,'Nourr Hamster',7,'false')");
        this->base.exec("insert into produit values(66,'Nourr Oiseau',7,'false')");
        this->base.exec("insert into produit values(67,'Nourr Lapin',7,'false')");
        this->base.exec("insert into produit values(68,'Laisse Chien',7,'false')");
        this->base.exec("insert into produit values(69,'Cage',7,'false')");

        this->base.exec("insert into produit values(8,'Chaussettes',8,'false')");
        this->base.exec("insert into produit values(81,'T Shirt',8,'false')");
        this->base.exec("insert into produit values(82,'Pull',8,'false')");
        this->base.exec("insert into produit values(83,'Sweat',8,'false')");
        this->base.exec("insert into produit values(84,'Jean',8,'false')");
        this->base.exec("insert into produit values(85,'Survetement',8,'false')");
        this->base.exec("insert into produit values(86,'Caleçon',8,'false')");
        this->base.exec("insert into produit values(87,'Slip',8,'false')");
        this->base.exec("insert into produit values(88,'Echarpe',8,'false')");
        this->base.exec("insert into produit values(89,'Bonnet',8,'false')");

        this->base.exec("insert into produit values(9,'Piles',9,'false')");
        this->base.exec("insert into produit values(91,'Brosse',9,'false')");
        this->base.exec("insert into produit values(92,'Souris',9,'false')");
        this->base.exec("insert into produit values(93,'Encre',9,'false')");
        this->base.exec("insert into produit values(94,'Rateau',9,'false')");
        this->base.exec("insert into produit values(95,'Balai',9,'false')");
        this->base.exec("insert into produit values(96,'Pots',9,'false')");
        this->base.exec("insert into produit values(97,'Pansements',9,'false')");
        this->base.exec("insert into produit values(98,'Sirop',9,'false')");
        this->base.exec("insert into produit values(99,'red bulls',9,'false')");
        this->base.exec("insert into produit values(100,'Sirop pour la toux',9,'false')");


        this->base.exec("insert into produit values(10,'Steak hachés',5,'false')");
        this->base.exec("insert into produit values(101,'Glaces',5,'false')");
        this->base.exec("insert into produit values(102,'Pizza surgelé',5,'false')");
        this->base.exec("insert into produit values(103,'Nuggets',5,'false')");
        this->base.exec("insert into produit values(104,'Raviolis',5,'false')");
        this->base.exec("insert into produit values(105,'Frites',5,'false')");
        this->base.exec("insert into produit values(106,'Poisson surgelé',5,'false')");
        this->base.exec("insert into produit values(107,'Pain',5,'false')");
        this->base.exec("insert into produit values(108,'Legumes',5,'false')");
        this->base.exec("insert into produit values(109,'Fruits',5,'false')");

        this->base.exec("insert into produit values(110,'Eau plate',10,'false')");
        this->base.exec("insert into produit values(111,'Eau gazuse',10,'false')");
        this->base.exec("insert into produit values(112,'Jus de Fruit',10,'false')");
        this->base.exec("insert into produit values(113,'Coca Cola',10,'false')");
        this->base.exec("insert into produit values(114,'Orangina',10,'false')");
        this->base.exec("insert into produit values(115,'Ice Tea',10,'false')");
        this->base.exec("insert into produit values(116,'Fanta',10,'false')");
        this->base.exec("insert into produit values(117,'Monster',10,'false')");
        this->base.exec("insert into produit values(118,'Sirop',10,'false')");
        this->base.exec("insert into produit values(119,'Red Bull',10,'false')");
        this->base.exec("insert into produit values(120,'Powerade',10,'false')");

        this->base.exec("insert into produit values(130,'Bierre',11,'false')");
        this->base.exec("insert into produit values(121,'Vin Rouge',11,'false')");
        this->base.exec("insert into produit values(122,'Vin Rosé',11,'false')");
        this->base.exec("insert into produit values(123,'Vin Blanc',11,'false')");
        this->base.exec("insert into produit values(124,'Whisky',11,'false')");
        this->base.exec("insert into produit values(125,'Vodka',11,'false')");
        this->base.exec("insert into produit values(126,'Cointreau',11,'false')");
        this->base.exec("insert into produit values(127,'Clavados',11,'false')");
        this->base.exec("insert into produit values(128,'Get 27',11,'false')");
        this->base.exec("insert into produit values(129,'Chartreuse',11,'false')");

        this->base.exec("insert into produit values(140,'Nutella',12,'false')");
        this->base.exec("insert into produit values(131,'Confiture',12,'false')");
        this->base.exec("insert into produit values(132,'Miel',12,'false')");
        this->base.exec("insert into produit values(133,'Cereales',12,'false')");
        this->base.exec("insert into produit values(134,'Cracottes',12,'false')");
        this->base.exec("insert into produit values(135,'Biscottes',12,'false')");
        this->base.exec("insert into produit values(136,'Cacao',12,'false')");
        this->base.exec("insert into produit values(137,'Thé',12,'false')");
        this->base.exec("insert into produit values(138,'Café',12,'false')");
        this->base.exec("insert into produit values(139,'Chicoré',12,'false')");







        //insertion ProdInListe





     this->base.exec("commit;"); //fin transaction
    }

}
void MainWindow::fermeBase()
{
    this->base.close();
}
void MainWindow::on_stackedWidgetAppli_currentChanged(int arg1)
{
    if(arg1==0)
    {
        chargerListe();
        onModifQuoi="Liste";
    }
    if(arg1==1)
    {
        chargerMagasin();
        onModifQuoi="Magasin";

    }
    if(arg1==2)
    {
        chargerListeProduits();
        onModifQuoi="Produit";

    }
    if(arg1==3)
    {
        chargerRayon();
        onModifQuoi="Rayon";

    }
    if(arg1==4)
    {
        onModifQuoi="ProduitInBdd";
        chargerAjoutProduit();
    }
    if(arg1==8)
    {
        onModifQuoi="ProduitFavoris";
        chargerFavoris();
    }
    if(arg1==9)
    {

        chargerOrdreRayon();
    }
}


void MainWindow::chargerListe()
{
    ui->listWidgetListes->clear();
    vecteurListes.clear();
    QSqlQuery req("select Magasin.libelle ,idListe,Liste.libelle,Magasin.idMagasin from liste inner join magasin on liste.idMagasin=magasin.idMagasin;");
    int i=0;
      while(req.next())
      {
          QListWidgetItem *item=new QListWidgetItem(req.value(2).toString());
          QLinearGradient linGrad;
          QSqlQuery req2("select isFinished from liste where idListe="+req.value(1).toString());
          req2.first();
          if(req2.value(0).toBool())item->setText(req.value(2).toString()+" (Liste Complete)");
          linGrad.setStart(0,ui->listWidgetListes->y());
          linGrad.setFinalStop(ui->listWidgetListes->width(),ui->listWidgetListes->y());
          if(i%2==0)// defini la couleur , on change de couleur 1 item sur 2
          {
              linGrad.setColorAt( 0, QColor(191,230,244));
              linGrad.setColorAt( 1, Qt::white );


            }
          else
          {

                linGrad.setColorAt( 0, QColor(157,214,237));
                 linGrad.setColorAt( 1, Qt::white );
          }

            i++;

              QBrush brush(linGrad);
          item->setBackground(brush);
          QPixmap imagedisplay(getPath3(req.value(3).toInt()));
          QIcon icon(imagedisplay.scaled(QSize(100,100)));
           item->setIcon(icon);

          ui->listWidgetListes->addItem(item);
          vecteurListes.push_back(req.value(1).toString());

      }
      if(i==0)
      {
          ui->listWidgetListes->addItem("Vous n'avez pas encore de liste , vous pouvez en ajouter une en cliquant sur 'Ajouter une liste' ");
          ui->listWidgetListes->item(0)->setFlags(Qt::NoItemFlags);
          ui->listWidgetListes->item(0)->setForeground(QBrush(Qt::black));


      }
}
void MainWindow::chargerMagasin()
{
    compteur=0;
    ui->listWidgetMagasin->clear();
    QSqlQuery req("select libelle,idMagasin from magasin;");
    int i=0;

      while(req.next())
      {
          QListWidgetItem *item=new QListWidgetItem(req.value(0).toString());
          QLinearGradient linGrad;
          QPixmap imagedisplay(getPath3(req.value(1).toInt()));
          QIcon icon(imagedisplay.scaled(QSize(100,100)));
           item->setIcon(icon);
          linGrad.setStart(0,ui->listWidgetMagasin->y());
          linGrad.setFinalStop(ui->listWidgetMagasin->width(),ui->listWidgetMagasin->y());
          if(i%2==0)// defini la couleur , on change de couleur 1 item sur 2
          {
              linGrad.setColorAt( 0, QColor(191,230,244));
              linGrad.setColorAt( 1, Qt::white );


            }
          else
          {

                linGrad.setColorAt( 0, QColor(157,214,237));
                 linGrad.setColorAt( 1, Qt::white );
          }
            i++;

              QBrush brush(linGrad);

          item->setBackground(brush);
          ui->listWidgetMagasin->addItem(item);


      }
}
void MainWindow::creaNewListe(QString mag)
{
    QSqlQuery req("select max(idLIste) from liste");
    req.first();
    int idMax=req.value(0).toInt()+1;
    QSqlQuery req2("select idMagasin from magasin where libelle='"+mag+"'");
    req2.first();
    int idMag=req2.value(0).toInt();
    QSqlQuery req3("insert into liste values("+QString::number(idMax)+" ,"+QString::number(idMag)+" , '"+mag+"' , 'false')");
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(1);
}



void MainWindow::on_listWidgetMagasin_itemPressed(QListWidgetItem *item)
{
    timer4->start(1500);
    currentItem=ui->listWidgetMagasin->currentItem()->text();

}



void MainWindow::on_listWidgetListes_itemPressed(QListWidgetItem *item)
{
        timer->start(1800);
        currentItem=ui->listWidgetListes->currentItem()->text();
}
void MainWindow::afficheOptionListe()
{
    if(currentItem==ui->listWidgetListes->currentItem()->text())
    {
    DialogModifLIste OptionListe(this);
        //on fait passer les informations
        //on veut placer la fenetre correctement
    OptionListe.table="liste";
    OptionListe.champ1="idListe";
    OptionListe.id1=vecteurListes.value(ui->listWidgetListes->currentRow());
    OptionListe.champ2=maListe;
    OptionListe.id2="nullos";
            QDesktopWidget *desktop = QApplication::desktop();
            int screenWidth, width;
            int screenHeight, height;
            int x, y;
            QSize windowSize;
            screenWidth = desktop->width(); //     obtenir la largeur de l'écran
            screenHeight = desktop->height(); // obtenir la hauteur de l'écran

           //peu de calculs
            x = screenWidth /4 ;
            y = screenHeight / 2;
            y -= 100;

            OptionListe.move( x, y );// déplace la fenêtre de coordonnées


        OptionListe.exec();
    }

}

void MainWindow::afficheOptionRayon()
{
    if(currentItem==ui->listWidgetChoixRayon->currentItem()->text())
    {
    DialogModifLIste OptionListe(this);
        //on fait passer les informations
        //on veut placer la fenetre correctement
    OptionListe.table="Rayon";
    OptionListe.champ1="idRayon";
    OptionListe.id1=vecteurRayons.value(ui->listWidgetChoixRayon->currentRow());
    OptionListe.champ2="null";
    OptionListe.id2="null";
            QDesktopWidget *desktop = QApplication::desktop();
            int screenWidth, width;
            int screenHeight, height;
            int x, y;
            QSize windowSize;
            screenWidth = desktop->width(); //     obtenir la largeur de l'écran
            screenHeight = desktop->height(); // obtenir la hauteur de l'écran

           //peu de calculs
            x = screenWidth /4 ;
            y = screenHeight / 2;
            y -= 100;

            OptionListe.move( x, y );// déplace la fenêtre de coordonnées


            OptionListe.exec();
       }

}

void MainWindow::afficheOptionMagasin()
{
    if(currentItem==ui->listWidgetMagasin->currentItem()->text())
    {
    DialogModifLIste OptionListe(this);
        //on fait passer les informations
        //on veut placer la fenetre correctement
    OptionListe.table="Magasin";
    OptionListe.champ1="libelle";
    OptionListe.id1="'"+ui->listWidgetMagasin->currentItem()->text()+"'";
    OptionListe.champ2="null";
    OptionListe.id2="nullatre";
            QDesktopWidget *desktop = QApplication::desktop();
            int screenWidth, width;
            int screenHeight, height;
            int x, y;
            QSize windowSize;
            screenWidth = desktop->width(); //     obtenir la largeur de l'écran
            screenHeight = desktop->height(); // obtenir la hauteur de l'écran

           //peu de calculs
            x = screenWidth /4 ;
            y = screenHeight / 2;
            y -= 100;

            OptionListe.move( x, y );// déplace la fenêtre de coordonnées


            OptionListe.exec();
    }
}

void MainWindow::afficheOptionProduitInBdd()
{
    if(currentItem==ui->listWidgetAjoutProduit->currentItem()->text())
    {
    DialogModifLIste OptionListe(this);
        //on fait passer les informations
        //on veut placer la fenetre correctement
    OptionListe.table="Produit";
    OptionListe.champ1="idProd";
    OptionListe.id1=vecteurAjoutProd.value(ui->listWidgetAjoutProduit->currentRow());
    OptionListe.champ2="null";
            QDesktopWidget *desktop = QApplication::desktop();
            int screenWidth, width;
            int screenHeight, height;
            int x, y;
            QSize windowSize;
            screenWidth = desktop->width(); //     obtenir la largeur de l'écran
            screenHeight = desktop->height(); // obtenir la hauteur de l'écran

           //peu de calculs
            x = screenWidth /4 ;
            y = screenHeight / 2;
            y -= 100;

            OptionListe.move( x, y );// déplace la fenêtre de coordonnées


        OptionListe.exec();
    }
}

void MainWindow::afficheOptionProduitFavoris()
{
    if(currentItem==ui->listWidgetFavoris->currentItem()->text())
    {
    DialogModifLIste OptionListe(this,"Supprimer des Favoris");
        //on fait passer les informations
        //on veut placer la fenetre correctement
    OptionListe.table="Produit";
    OptionListe.champ1="idProd";
    OptionListe.id1=vecteuFavoris.value(ui->listWidgetFavoris->currentRow());
    OptionListe.champ2="favoris";

            QDesktopWidget *desktop = QApplication::desktop();
            int screenWidth, width;
            int screenHeight, height;
            int x, y;
            QSize windowSize;
            screenWidth = desktop->width(); //     obtenir la largeur de l'écran
            screenHeight = desktop->height(); // obtenir la hauteur de l'écran

           //peu de calculs
            x = screenWidth /4 ;
            y = screenHeight / 2;
            y -= 100;

            OptionListe.move( x, y );// déplace la fenêtre de coordonnées


        OptionListe.exec();
      }
}

void MainWindow::on_listWidgetListes_itemClicked(QListWidgetItem *item)
{
    timer->stop();
   maListe=vecteurListes.value(ui->listWidgetListes->currentRow());
   QSqlQuery req("select isFinished from liste where idListe="+maListe);
   req.first();

   ui->stackedWidgetAppli->setCurrentIndex(2);
}
void MainWindow::chargerListeProduits()
{
    ui->listWidgetProduitsListe->clear();
    currentRayonAffiche="null";
    vecteuProdInListe.clear();
        int listeFini=0;
        int i=0;
      QLinearGradient linGrad;
      linGrad.setStart(0,ui->listWidgetProduitsListe->y());// gradiant
      linGrad.setFinalStop(ui->listWidgetProduitsListe->width(),ui->listWidgetProduitsListe->y());// stop gradiant
    QSqlQuery req("select Produit.libelle,isBought,prodInList.idProd,qte,Rayon.libelle from Produit inner join prodInList on Produit.idProd=prodInList.idProd inner join Rayon on Produit.idRayon=Rayon.idRayon where idListe="+maListe+" order by ordre");
    while(req.next())
    {
        i++;
        if(req.value(4).toString()!=currentRayonAffiche)
        {
            currentRayonAffiche=req.value(4).toString();
            QListWidgetItem *itemRayon=new QListWidgetItem(req.value(4).toString());
            int RayonFini=0;

            QSqlQuery req2("select isBought from prodInList inner join Produit on prodInList.idProd=Produit.idProd inner join Rayon on Produit.idRayon=Rayon.idRayon where Rayon.libelle='"+req.value(4).toString()+"'");
            while(req2.next())
            {
                if(req2.value(0).toBool()==false)RayonFini++;
            }
            QPixmap imagedisplay(":/Images/rouge.jpg");
            QIcon icon(imagedisplay.scaled(QSize(120,120)));
            itemRayon->setIcon(icon);
            if(RayonFini==0)
             {

                itemRayon->setText(req.value(4).toString()+" Rayon Complet");
                QPixmap imagedisplay2(":/Images/ok.jpg");
                QIcon icon2(imagedisplay2.scaled(QSize(120,120)));
                itemRayon->setIcon(icon2);


            }

            linGrad.setColorAt( 0, QColor(200,200,200));//couleur
            linGrad.setColorAt( 1, Qt::white );
            QBrush brush(linGrad);//definit une QBrush
            itemRayon->setForeground(QBrush(Qt::black));
             itemRayon->setBackground(brush);
            listeFini+=RayonFini;

            ui->listWidgetProduitsListe->addItem(itemRayon);
            vecteuProdInListe.push_back("null");

        }

        QListWidgetItem *myItem=new QListWidgetItem(req.value(3).toString()+" "+req.value(0).toString());
        QPixmap imagedisplay(getPath2(req.value(2).toInt()));
        QIcon icon(imagedisplay.scaled(QSize(120,120)));
        myItem->setIcon(icon);

        if(req.value(1).toBool())
        {
            linGrad.setColorAt( 0, QColor(40,226,68));//couleur
            linGrad.setColorAt( 1, Qt::white );
            myItem->setText(myItem->text()+" | Acheté");
            myItem->setCheckState(Qt::Checked);

          }
        else
        {

              linGrad.setColorAt( 0, QColor(226,53,22));
               linGrad.setColorAt( 1, Qt::white );
               myItem->setText(myItem->text()+" | Pas Acheté");
               myItem->setCheckState(Qt::Unchecked);

        }


          QBrush brush(linGrad);//definit une QBrush

        myItem->setBackground(brush);//ajout de la couleur dd l item


        ui->listWidgetProduitsListe->addItem(myItem);
        vecteuProdInListe.push_back(req.value(2).toString());
        }
    if(i==0)
    {
        ui->listWidgetProduitsListe->addItem("Vous n'avez pas encore de produits dans liste , vous pouvez en ajouter une en cliquant sur 'Produits' ");
        ui->listWidgetProduitsListe->item(0)->setFlags(Qt::NoItemFlags);
        ui->listWidgetProduitsListe->item(0)->setForeground(QBrush(Qt::black));
    }
    actualiserInfo(listeFini,i);
 }
void MainWindow::actualiserInfo(int listeFini,int i)
{
        if(listeFini==0 and i!=0)
        {
            QSqlQuery req3("update liste set isFinished='true' where idListe="+maListe);
            QSqlQuery req4("select libelle from liste where idListe="+maListe);
            req4.first();

            ui->labelListeName->setText(req4.value(0).toString()+"   (Liste Complete)");
        }
        else
        {
            QSqlQuery req3("update liste set isFinished='false' where idListe="+maListe);            
            QSqlQuery req4("select libelle from liste where idListe="+maListe);
            req4.first();

            ui->labelListeName->setText(req4.value(0).toString());
        }

}
void MainWindow::chargerRayon()
{
    vecteurRayons.clear();
    ui->listWidgetChoixRayon->clear();
    int i=0;
    QSqlQuery req("select libelle,idRayon from Rayon order by ordre");
    while(req.next())
    {
        QListWidgetItem *item=new QListWidgetItem(req.value(0).toString());
        QLinearGradient linGrad;
        QPixmap imagedisplay(getPath(req.value(1).toInt()));
        QIcon icon(imagedisplay.scaled(QSize(120,120)));
        item->setIcon(icon);
        linGrad.setStart(0,ui->listWidgetChoixRayon->y());
        linGrad.setFinalStop(ui->listWidgetChoixRayon->width(),ui->listWidgetChoixRayon->y());
        if(i%2==0)// defini la couleur , on change de couleur 1 item sur 2
        {
            linGrad.setColorAt( 0, QColor(191,230,244));
            linGrad.setColorAt( 1, Qt::white );


          }
        else
        {

              linGrad.setColorAt( 0, QColor(157,214,237));
               linGrad.setColorAt( 1, Qt::white );
        }
          i++;

            QBrush brush(linGrad);

        item->setBackground(brush);


        ui->listWidgetChoixRayon->addItem(item);
        vecteurRayons.push_back(req.value(1).toString());
    }

}
void MainWindow::on_pushButtonProduitsListe_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(3);
}
void MainWindow::on_pushButtonProduitsListe_2_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(3);

}
void MainWindow::on_pushButtonProduitsListe_3_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(3);

}
void MainWindow::on_pushButtonLIste_3_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(2);

}

void MainWindow::on_pushButtonLIste_2_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(2);

}

void MainWindow::on_pushButtonLIste_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(2);

}

void MainWindow::on_listWidgetChoixRayon_itemClicked(QListWidgetItem *item)
{
    rayonChoisi=vecteurRayons.value(ui->listWidgetChoixRayon->currentRow());
    timer3->stop();
    ui->stackedWidgetAppli->setCurrentIndex(4);


}
void MainWindow::chargerAjoutProduit()
{
    ui->listWidgetAjoutProduit->clear();
    vecteurAjoutProd.clear();

    QSqlQuery req2("select idProd , qte from prodInList where idListe="+maListe);
    QVector <QString> vId;
    QVector <QString> vqt;
    vId.clear();
    vqt.clear();
    while(req2.next())
    {
        vId.push_back(req2.value(0).toString());
        vqt.push_back(req2.value(1).toString());
    }
    int i=0;
    QSqlQuery req("select libelle,idProd from Produit where idRayon="+rayonChoisi+" order by libelle");
    while(req.next())
    {


        QListWidgetItem *item=new QListWidgetItem(req.value(0).toString());
        QPixmap imagedisplay(getPath2(req.value(1).toInt()));
        QIcon icon(imagedisplay.scaled(QSize(100,100)));
         item->setIcon(icon);
        for(int n=0;n<vId.size();n++)
        {
            if(vId.value(n)==req.value(1).toString())
            {
                item->setText(req.value(0).toString()+" ("+vqt.value(n)+")");
            }
        }
        QLinearGradient linGrad;
        linGrad.setStart(0,ui->listWidgetAjoutProduit->y());

        linGrad.setFinalStop(ui->listWidgetAjoutProduit->width(),ui->listWidgetAjoutProduit->y());
        if(i%2==0)// defini la couleur , on change de couleur 1 item sur 2
        {
            linGrad.setColorAt( 0, QColor(191,230,244));
            linGrad.setColorAt( 1, Qt::white );


          }
        else
        {

              linGrad.setColorAt( 0, QColor(157,214,237));
               linGrad.setColorAt( 1, Qt::white );
        }
          i++;

            QBrush brush(linGrad);

        item->setBackground(brush);

        ui->listWidgetAjoutProduit->addItem(item);
        vecteurAjoutProd.push_back(req.value(1).toString());

    }

}

void MainWindow::on_pushButtonBack2_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(0);
}

void MainWindow::on_pushButtonBack3_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(0);

}

void MainWindow::on_pushButtonBack4_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(2);

}

void MainWindow::on_pushButtonBack5_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(3);

}

void MainWindow::on_listWidgetAjoutProduit_itemClicked(QListWidgetItem *item)
{
            timer5->stop();

           idNewProd=vecteurAjoutProd.value(ui->listWidgetAjoutProduit->currentRow());
              DialogQteNewProd AjoutProd(this);
            //on fait passer les informations
            AjoutProd.getProd(vecteurAjoutProd.value(ui->listWidgetAjoutProduit->currentRow()),maListe);
            idNewProd=vecteurAjoutProd.value(ui->listWidgetAjoutProduit->currentRow());
            newProd=true;
            if(ui->listWidgetAjoutProduit->currentItem()->text().at(ui->listWidgetAjoutProduit->currentItem()->text().length()-1)==')')
            {
                newProd=false;
                qteNewProd=ui->listWidgetAjoutProduit->currentItem()->text().at(ui->listWidgetAjoutProduit->currentItem()->text().length()-2);
            }
            AjoutProd.getProd(vecteurAjoutProd.value(ui->listWidgetAjoutProduit->currentRow()),maListe);
            //on veut placer la fenetre correctement

                QDesktopWidget *desktop2 = QApplication::desktop();
                int screenWidth, width;
                int screenHeight, height;
                int x, y;
                QSize windowSize;
                screenWidth = desktop2->width(); //     obtenir la largeur de l'écran
                screenHeight = desktop2->height(); // obtenir la hauteur de l'écran

               //peu de calculs
                y = screenHeight / 2;


                AjoutProd.move( 35, y);



            AjoutProd.exec();




}

void MainWindow::on_listWidgetProduitsListe_itemClicked(QListWidgetItem *item)
{
    timer2->stop();
    QSqlQuery req3("select libelle from Rayon");
    while(req3.next())
    {
        if(currentItem==req3.value(0).toString() or currentItem==req3.value(0).toString()+" Rayon Complet")
        {
            currentItem="rayonOnAffichePas";
        }
    }

    if(currentItem==ui->listWidgetProduitsListe->currentItem()->text())
    {
    QSqlQuery req2("select isBought from prodInList where idProd="+vecteuProdInListe.value(ui->listWidgetProduitsListe->currentRow())+" and idListe="+maListe);
    req2.first();
    if(req2.value(0).toBool())
    {
        QSqlQuery req("update prodInList set isBought='false' where idProd="+vecteuProdInListe.value(ui->listWidgetProduitsListe->currentRow())+" and idListe="+maListe);

    }
    if(req2.value(0).toBool()==false)
    {
        QSqlQuery req("update prodInList set isBought='true' where idProd="+vecteuProdInListe.value(ui->listWidgetProduitsListe->currentRow())+" and idListe="+maListe);

    }
    chargerListeProduits();
    }

}




void MainWindow::on_listWidgetProduitsListe_itemPressed(QListWidgetItem *item)
{
    currentItem=ui->listWidgetProduitsListe->currentItem()->text();
    timer2->start(1500);
}

void MainWindow::on_pushButtonAjoutProddBdd_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(5);
    ui->lineEditAjoutProd->clear();
   // ui->labelAjoutProduit->setText("Ajouter un produit - Rayon "+ui->listWidgetChoixRayon->currentItem()->text());
}


void MainWindow::on_pushButtonAjoutProduit_clicked()
{
    QSqlQuery req2("select max(idProd) from Produit");
    req2.first();
    QString idProd=QString::number(req2.value(0).toInt()+1);
    QSqlQuery req("insert into Produit values("+idProd+" ,'"+ui->lineEditAjoutProd->text()+"' , "+rayonChoisi+" , '"+AjoutIsFav+"' )");
    ui->stackedWidgetAppli->setCurrentIndex(4);
}

void MainWindow::on_lineEditAjoutProd_cursorPositionChanged(int arg1, int arg2)
{
    if(ui->lineEditAjoutProd->text().length()>0)
    {
        ui->pushButtonAjoutProduit->setEnabled(true);
        ui->pushButtonAjoutProduit2->setEnabled(true);
    }
    else
    {
        ui->pushButtonAjoutProduit->setEnabled(false);
        ui->pushButtonAjoutProduit2->setEnabled(false);
    }
}

void MainWindow::on_pushButtonAjoutProduit2_clicked()
{
    on_pushButtonAjoutProduit_clicked();
    ui->stackedWidgetAppli->setCurrentIndex(5);

    QSqlQuery req2("select max(idProd) from Produit");
    req2.first();
    QString idProd=req2.value(0).toString();

    DialogQteNewProd AjoutProd(this);

  //on fait passer les informations
  AjoutProd.getProd(idProd,maListe);

    newProd=true;

  //on veut placer la fenetre correctement

      QDesktopWidget *desktop4 = QApplication::desktop();
      int screenWidth, width;
      int screenHeight, height;
      int x, y;
      QSize windowSize;
      screenWidth = desktop4->width(); //     obtenir la largeur de l'écran
      screenHeight = desktop4->height(); // obtenir la hauteur de l'écran

     //peu de calculs
      y = screenHeight / 2;


      AjoutProd.move( 35, y);



  AjoutProd.exec();
  ui->stackedWidgetAppli->setCurrentIndex(4);

}

void MainWindow::on_pushButtonAnnulerAjoutProd_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(4);
}

void MainWindow::on_pushButtonChangerFav_clicked()
{
    if(ui->labelIsFav->text()=="Favoris : NON")
    {
        ui->labelIsFav->setText("Favoris : OUI");
        AjoutIsFav="true";
    }
    else
    {
        AjoutIsFav="false";
        ui->labelIsFav->setText("Favoris : NON");
    }

}

void MainWindow::on_listWidgetChoixRayon_itemPressed(QListWidgetItem *item)
{
    timer3->start(1500);
    currentItem=ui->listWidgetChoixRayon->currentItem()->text();
}

void MainWindow::on_listWidgetMagasin_itemClicked(QListWidgetItem *item)
{
    timer4->stop();
    compteur++;
    if(compteur<2)
    {
    creaNewListe(item->text());
    }

     ui->stackedWidgetAppli->setCurrentIndex(0);
}

void MainWindow::on_listWidgetAjoutProduit_itemPressed(QListWidgetItem *item)
{
    currentItem=ui->listWidgetAjoutProduit->currentItem()->text();
    timer5->start(1500);

}

void MainWindow::on_pushButtonAjouterMagasin_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(6);
    ui->labelAjoutMagRay->setText("Nom du Magasin : ");
    AjoutMagRayTable="Magasin";
    ui->labelAjouterQuoiMagRay->setText("Ajouter un Magasin");

    ui->lineEditAjoutMagRay->clear();
}
void MainWindow::on_pushButtonAjoutRayon_clicked()
{    ui->stackedWidgetAppli->setCurrentIndex(6);
     ui->labelAjoutMagRay->setText("Nom du Rayon : ");
     AjoutMagRayTable="Rayon";
      ui->labelAjouterQuoiMagRay->setText("Ajouter un Rayon");
      ui->lineEditAjoutMagRay->clear();
}
void MainWindow::on_pushButtonAjouterRayMag_clicked()
{
    if(AjoutMagRayTable=="Magasin")
    {
        QSqlQuery req("select max(idMagasin) from Magasin");
        req.first();
        QString idMag=QString::number(req.value(0).toInt()+1);
        QSqlQuery req2("insert into Magasin values("+idMag+" , '"+ui->lineEditAjoutMagRay->text()+"')");
        ui->stackedWidgetAppli->setCurrentIndex(1);
    }
    if(AjoutMagRayTable=="Rayon")
    {
        QSqlQuery req("select max(idRayon) , max(ordre) from Rayon");
        req.first();
        QString idRayon=QString::number(req.value(0).toInt()+1);
        QString ordre=QString::number(req.value(1).toInt()+1);

        QSqlQuery req2("insert into Rayon values("+idRayon+" , '"+ui->lineEditAjoutMagRay->text()+"' , "+ordre+")");
        ui->stackedWidgetAppli->setCurrentIndex(3);
    }
}

void MainWindow::on_lineEditAjoutMagRay_cursorPositionChanged(int arg1, int arg2)
{
    if(ui->lineEditAjoutMagRay->text().length()>0)
    {
        ui->pushButtonAjouterRayMag->setEnabled(true);
    }
}

void MainWindow::on_pushButtonAnnulerAjoutMagRay_clicked()
{
    if(AjoutMagRayTable=="Rayon")
    {
        ui->stackedWidgetAppli->setCurrentIndex(3);
    }
    else
    {
        ui->stackedWidgetAppli->setCurrentIndex(1);

    }
}
void MainWindow::gererModif()
{
    ui->stackedWidgetAppli->setCurrentIndex(7);
    if(onModifQuoi=="Liste")
    {
        ui->labelModifierAll->setText("Modifier la Liste");
        ui->labelModifierAll2->setText("Nom de la Liste :");
        ui->lineEditModifierLibelleAll->setText(ui->listWidgetListes->currentItem()->text());
    }
    if(onModifQuoi=="Magasin")
    {
        ui->labelModifierAll->setText("Modifier le Magasin");
        ui->labelModifierAll2->setText("Nom du Magasin :");
        ui->lineEditModifierLibelleAll->setText(ui->listWidgetMagasin->currentItem()->text());
    }
    if(onModifQuoi=="Produit")
    {
        ui->labelModifierAll->setText("Modifier le Produit");
        ui->labelModifierAll2->setText("Quantite :");
        QSqlQuery req("select qte from prodInList where idProd="+vecteuProdInListe.value(ui->listWidgetProduitsListe->currentRow())+" and idListe="+maListe);
        req.first();
        ui->pushButtonModifierQtePlus->setVisible(true);
        ui->pushButtonModifierQTeMoins->setVisible(true);
        ui->lineEditModifierLibelleAll->setText(req.value(0).toString());
    }
    else
    {
        ui->pushButtonModifierQtePlus->setVisible(false);
        ui->pushButtonModifierQTeMoins->setVisible(false);
    }
    if(onModifQuoi=="Rayon")
    {
        ui->labelModifierAll->setText("Modifier le Rayon");
        ui->labelModifierAll2->setText("Nom du Rayon :");
        ui->lineEditModifierLibelleAll->setText(ui->listWidgetChoixRayon->currentItem()->text());
    }
    if(onModifQuoi=="ProduitInBdd" or onModifQuoi=="ProduitFavoris")
    {
       if(onModifQuoi=="ProduitInBdd")
       {
        ui->labelModifierAll->setText("Modifier le Produit");
        ui->labelModifierAll2->setText("Nom du Produit :");
        QSqlQuery req("select isFavoris,libelle from Produit where idProd="+vecteurAjoutProd.value(ui->listWidgetAjoutProduit->currentRow()));
                req.first();
        if(req.value(0).toBool())
        {
            ui->labelIsFavModifier->setText("Favoris : OUI");
        }
        else
        {
            ui->labelIsFavModifier->setText("Favoris : NON");

        }
        ui->lineEditModifierLibelleAll->setText(req.value(1).toString());
        ui->pushButtonChangerFavModifier->setVisible(true);
        ui->labelIsFavModifier->setVisible(true);
       }


        if(onModifQuoi=="ProduitFavoris")
         {
           ui->labelModifierAll->setText("Modifier le Produit");
           ui->labelModifierAll2->setText("Nom du Produit :");
            QSqlQuery req("select isFavoris,libelle from Produit where idProd="+vecteuFavoris.value(ui->listWidgetFavoris->currentRow()));
                req.first();
            if(req.value(0).toBool())
              {
               ui->labelIsFavModifier->setText("Favoris : OUI");
                }
              else
              {
              ui->labelIsFavModifier->setText("Favoris : NON");

               }
            ui->lineEditModifierLibelleAll->setText(req.value(1).toString());
               ui->pushButtonChangerFavModifier->setVisible(true);
               ui->labelIsFavModifier->setVisible(true);
        }
       }
    else
    {
        ui->pushButtonChangerFavModifier->setVisible(false);
        ui->labelIsFavModifier->setVisible(false);
    }

}
void MainWindow::on_pushButtonModifierAll_clicked()
{
    if(onModifQuoi=="Liste")
    {
        QSqlQuery req("update Liste set libelle='"+ui->lineEditModifierLibelleAll->text()+"' where idListe="+vecteurListes.value(ui->listWidgetListes->currentRow()));
        ui->stackedWidgetAppli->setCurrentIndex(0);
    }
    if(onModifQuoi=="Magasin")
    {
        QSqlQuery req("update Magasin set libelle='"+ui->lineEditModifierLibelleAll->text()+"' where libelle="+ui->listWidgetMagasin->currentItem()->text());

        ui->stackedWidgetAppli->setCurrentIndex(1);

    }
    if(onModifQuoi=="Produit")
    {
        QSqlQuery req("update prodInList set qte="+ui->lineEditModifierLibelleAll->text()+" where idListe="+maListe+" and idProd="+vecteuProdInListe.value(ui->listWidgetProduitsListe->currentRow()));
        ui->stackedWidgetAppli->setCurrentIndex(2);

    }
    if(onModifQuoi=="Rayon")
    {
        QSqlQuery req("update Rayon set libelle='"+ui->lineEditModifierLibelleAll->text()+"' where idRayon="+vecteurRayons.value(ui->listWidgetChoixRayon->currentRow()));
        ui->stackedWidgetAppli->setCurrentIndex(3);

    }
    if(onModifQuoi=="ProduitInBdd")
    {
        QString isFav="false";
        if(ui->labelIsFavModifier->text()=="Favoris : OUI")isFav="true";
        QSqlQuery req("update Produit set libelle='"+ui->lineEditModifierLibelleAll->text()+"' , isFavoris='"+isFav+"' where idProd="+vecteurAjoutProd.value(ui->listWidgetAjoutProduit->currentRow()));
        ui->stackedWidgetAppli->setCurrentIndex(4);

    }
    if(onModifQuoi=="ProduitFavoris")
    {
        QString isFav="false";
        if(ui->labelIsFavModifier->text()=="Favoris : OUI")isFav="true";
        QSqlQuery req("update Produit set libelle='"+ui->lineEditModifierLibelleAll->text()+"' , isFavoris='"+isFav+"' where idProd="+vecteuFavoris.value(ui->listWidgetFavoris->currentRow()));
        ui->stackedWidgetAppli->setCurrentIndex(8);

    }
}
void MainWindow::on_pushButtonModifierAnnulerAll_clicked()
{
    if(onModifQuoi=="Liste")
    {
        ui->stackedWidgetAppli->setCurrentIndex(0);
    }
    if(onModifQuoi=="Magasin")
    {
        ui->stackedWidgetAppli->setCurrentIndex(1);

    }
    if(onModifQuoi=="Produit")
    {
        ui->stackedWidgetAppli->setCurrentIndex(2);

    }
    if(onModifQuoi=="Rayon")
    {
        ui->stackedWidgetAppli->setCurrentIndex(3);

    }
    if(onModifQuoi=="ProduitInBdd")
    {
        ui->stackedWidgetAppli->setCurrentIndex(4);

    }
    if(onModifQuoi=="ProduitFavoris")
    {
        ui->stackedWidgetAppli->setCurrentIndex(8);

    }
}



void MainWindow::on_pushButtonModifierQtePlus_clicked()
{
    ui->lineEditModifierLibelleAll->setText(QString::number(ui->lineEditModifierLibelleAll->text().toInt()+1));

}

void MainWindow::on_pushButtonModifierQTeMoins_clicked()
{
    ui->lineEditModifierLibelleAll->setText(QString::number(ui->lineEditModifierLibelleAll->text().toInt()-1));

}

void MainWindow::on_pushButtonChangerFavModifier_clicked()
{
    if(ui->labelIsFavModifier->text()=="Favoris : NON")
    {
        ui->labelIsFavModifier->setText("Favoris : OUI");
    }
    else
    {
        ui->labelIsFavModifier->setText("Favoris : NON");
    }
}

void MainWindow::on_pushButtonLIste_9_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(2);
}

void MainWindow::on_pushButtonProduitsListe_9_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(3);
}

void MainWindow::on_pushButtonFavorisListe_9_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(8);
}
void MainWindow::on_pushButtonFavorisListe_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(8);
}

void MainWindow::on_pushButtonFavorisListe_2_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(8);
}

void MainWindow::on_pushButtonFavorisListe_3_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(8);
}

void MainWindow::chargerFavoris()
{
    ui->listWidgetFavoris->clear();
    vecteuFavoris.clear();
    QSqlQuery req2("select idProd , qte from prodInList where idListe="+maListe);
    QVector <QString> vId;
    QVector <QString> vqt;
    vId.clear();
    vqt.clear();
    while(req2.next())
    {
        vId.push_back(req2.value(0).toString());
        vqt.push_back(req2.value(1).toString());
    }
    int i=0;
    QSqlQuery req("select libelle,idProd from Produit where isFavoris='true' order by libelle");
    while(req.next())
    {


        QListWidgetItem *item=new QListWidgetItem(req.value(0).toString());
        QPixmap imagedisplay(getPath2(req.value(1).toInt()));
        QIcon icon(imagedisplay.scaled(QSize(100,100)));
         item->setIcon(icon);
        for(int n=0;n<vId.size();n++)
        {
            if(vId.value(n)==req.value(1).toString())
            {
                item->setText(req.value(0).toString()+" ("+vqt.value(n)+")");
            }
        }
        QLinearGradient linGrad;
        linGrad.setStart(0,ui->listWidgetFavoris->y());

        linGrad.setFinalStop(ui->listWidgetFavoris->width(),ui->listWidgetFavoris->y());
        if(i%2==0)// defini la couleur , on change de couleur 1 item sur 2
        {
            linGrad.setColorAt( 0, QColor(191,230,244));
            linGrad.setColorAt( 1, Qt::white );


          }
        else
        {

              linGrad.setColorAt( 0, QColor(157,214,237));
               linGrad.setColorAt( 1, Qt::white );
        }
          i++;

            QBrush brush(linGrad);

        item->setBackground(brush);

        ui->listWidgetFavoris->addItem(item);
        vecteuFavoris.push_back(req.value(1).toString());

    }
    if(i==0)
    {
        ui->listWidgetFavoris->addItem("Vous n'avez pas encore de favoris , vous pouvez en ajouter en allant sur Produit=>Choix du Rayon => Modifier => Favoris Oui");
        ui->listWidgetFavoris->item(0)->setFlags(Qt::NoItemFlags);
        ui->listWidgetFavoris->item(0)->setForeground(QBrush(Qt::black));


    }

}

void MainWindow::chargerOrdreRayon()
{
    vecteuOrdreRayon.clear();
    ui->listWidgetOrderRayon->clear();
    int i=0;
    QSqlQuery req("select libelle,idRayon from Rayon order by ordre");
    while(req.next())
    {
        QListWidgetItem *item=new QListWidgetItem(req.value(0).toString());
        QLinearGradient linGrad;
        QPixmap imagedisplay(getPath(req.value(1).toInt()));
        QIcon icon(imagedisplay.scaled(QSize(120,120)));
        item->setIcon(icon);
        linGrad.setStart(0,ui->listWidgetOrderRayon->y());
        linGrad.setFinalStop(ui->listWidgetOrderRayon->width(),ui->listWidgetOrderRayon->y());
        if(i%2==0)// defini la couleur , on change de couleur 1 item sur 2
        {
            linGrad.setColorAt( 0, QColor(191,230,244));
            linGrad.setColorAt( 1, Qt::white );


          }
        else
        {

              linGrad.setColorAt( 0, QColor(157,214,237));
               linGrad.setColorAt( 1, Qt::white );
        }
          i++;

            QBrush brush(linGrad);

        item->setBackground(brush);


        ui->listWidgetOrderRayon->addItem(item);
        vecteuOrdreRayon.push_back(req.value(1).toString());
    }
}

QString MainWindow::getPath2(int arg)
{
    switch (arg) {
    case 27: return(":/Images/actimel.jpg"); break;
    case 23: return(":/Images/beurre.jpg"); break;
    case 29: return(":/Images/cremefraiche.jpg"); break;
    case 25: return(":/Images/danette.jpg"); break;
    case 26: return(":/Images/flan.jpg"); break;
    case 21: return(":/Images/fromage.jpg"); break;
    case 2: return(":/Images/lait.jpg"); break;
    case 22: return(":/Images/oeuf.jpg"); break;
    case 28: return(":/Images/pate.jpg"); break;
    case 24: return(":/Images/yaourt.gif"); break;

    case 4: return(":/Images/tomate.jpg"); break;
    case 41: return(":/Images/carotte.jpg"); break;
    case 42: return(":/Images/courgette.jpg"); break;
    case 43: return(":/Images/choux.jpg"); break;
    case 44: return(":/Images/aubergine.png"); break;
    case 45: return(":/Images/banane.png"); break;
    case 46: return(":/Images/abricot.jpg"); break;
    case 47: return(":/Images/pomme.png"); break;
    case 48: return(":/Images/poire.jpg"); break;
    case 49: return(":/Images/peche.jpg"); break;

    case 5: return(":/Images/pates.jpg"); break;
    case 51: return(":/Images/pizza.png"); break;
    case 52: return(":/Images/lasagne.jpg"); break;
    case 53: return(":/Images/quiche.jpg"); break;
    case 54: return(":/Images/nems.jpg"); break;
    case 55: return(":/Images/sandwitch.png"); break;
    case 56: return(":/Images/riz.jpg"); break;
    case 57: return(":/Images/taboule.jpg"); break;
    case 58: return(":/Images/soupe.jpg"); break;
    case 59: return(":/Images/couscous.jpg"); break;

    case 1: return(":/Images/bonbon.png"); break;
    case 11: return(":/Images/biscuit.jpg"); break;
    case 12: return(":/Images/chocolat.jpg"); break;
    case 13: return(":/Images/barrechoco.jpg"); break;
    case 14: return(":/Images/sucrerie.jpg"); break;
    case 15: return(":/Images/twix.jpg"); break;
    case 16: return(":/Images/mars.jpg"); break;
    case 17: return(":/Images/mm.jpg"); break;
    case 18: return(":/Images/smarties.jpg"); break;
    case 19: return(":/Images/nougat.jpg"); break;

    case 10: return(":/Images/steak.jpg"); break;
    case 101: return(":/Images/glace.jpeg"); break;
    case 102: return(":/Images/pizza.png"); break;
    case 103: return(":/Images/nuggets.jpg"); break;
    case 104: return(":/Images/ravioli.jpg"); break;
    case 105: return(":/Images/frites.jpg"); break;
    case 106: return(":/Images/poisson.jpg"); break;
    case 107: return(":/Images/pain.jpg"); break;
    case 108: return(":/Images/legumes.jpg"); break;
    case 109: return(":/Images/fruits.jpg"); break;

    case 3: return(":/Images/savon.jpg"); break;
    case 31: return(":/Images/parfum.jpg"); break;
    case 32: return(":/Images/hygiene.png"); break;
    case 33: return(":/Images/gel.jpg"); break;
    case 34: return(":/Images/brosse.jpg"); break;
    case 35: return(":/Images/mouchoirs.jpg"); break;
    case 36: return(":/Images/dentifrice.jpg"); break;
    case 37: return(":/Images/cotontige.jpg"); break;
    case 38: return(":/Images/rasoir.jpg"); break;
    case 39: return(":/Images/javel.jpg"); break;

    case 6: return(":/Images/croqette.jpg"); break;
    case 61: return(":/Images/litere.jpg"); break;
    case 62: return(":/Images/patechat.jpg"); break;
    case 63: return(":/Images/jeuchat.jpg"); break;
    case 64: return(":/Images/poissonnour.jpg"); break;
    case 65: return(":/Images/hamster.jpg"); break;
    case 66: return(":/Images/oiseau.jpg"); break;
    case 67: return(":/Images/lapin.jpg"); break;
    case 68: return(":/Images/laisse.jpg"); break;
    case 69: return(":/Images/cage.jpg"); break;

    case 8: return(":/Images/chaussettes.jpg"); break;
    case 81: return(":/Images/tshirt.png"); break;
    case 82: return(":/Images/pull.jpg"); break;
    case 83: return(":/Images/sweat.jpg"); break;
    case 84: return(":/Images/jean.jpg"); break;
    case 85: return(":/Images/survet.jpg"); break;
    case 86: return(":/Images/calecon.jpg"); break;
    case 87: return(":/Images/slip.jpg"); break;
    case 88: return(":/Images/echarpe.jpg"); break;
    case 89: return(":/Images/bonnet.jpg"); break;

    case 9: return(":/Images/piles.jpg"); break;
    case 91: return(":/Images/brosse2.jpg"); break;
    case 92: return(":/Images/souris.gif"); break;
    case 93: return(":/Images/encre.png"); break;
    case 94: return(":/Images/rateau.jpg"); break;
    case 95: return(":/Images/balai.jpg"); break;
    case 96: return(":/Images/pot.jpg"); break;
    case 97: return(":/Images/pansement.jpeg"); break;
    case 98: return(":/Images/pelle.png"); break;
    case 99: return(":/Images/souris.jpg"); break;
    case 100: return(":/Images/sirop.gif"); break;

    case 110: return(":/Images/eau.png"); break;
    case 111: return(":/Images/eau2.jpg"); break;
    case 112: return(":/Images/jus.jpg"); break;
    case 113: return(":/Images/coca.gif"); break;
    case 114: return(":/Images/orangina.jpg"); break;
    case 115: return(":/Images/icetea.png"); break;
    case 116: return(":/Images/fanta.png"); break;
    case 117: return(":/Images/monster.jpg"); break;
    case 118: return(":/Images/sirop.jpg"); break;
    case 119: return(":/Images/red bull.png"); break;
    case 120: return(":/Images/powerade.jpg"); break;

    case 130: return(":/Images/bierre.png"); break;
    case 121: return(":/Images/vin.jpg"); break;
    case 122: return(":/Images/vinrose.jpg"); break;
    case 123: return(":/Images/vinb.jpg"); break;
    case 124: return(":/Images/whisky.jpg"); break;
    case 125: return(":/Images/vodka.jpg"); break;
    case 126: return(":/Images/cointreau.jpg"); break;
    case 127: return(":/Images/calvados.jpg"); break;
    case 128: return(":/Images/get.jpg"); break;
    case 129: return(":/Images/chartreuse.jpg"); break;

    case 140: return(":/Images/nut.jpg"); break;
    case 131: return(":/Images/confiture.jpg"); break;
    case 132: return(":/Images/miel.jpg"); break;
    case 133: return(":/Images/cereales.jpg"); break;
    case 134: return(":/Images/cracottes.jpg"); break;
    case 135: return(":/Images/biscotte.jpg"); break;
    case 136: return(":/Images/cacao.jpg"); break;
    case 137: return(":/Images/the.jpg"); break;
    case 138: return(":/Images/cafe.jpg"); break;
    case 139: return(":/Images/ricore.jpg"); break;


    default:return ":/Images/autre.png";
        break;
     }


}

QString MainWindow::getPath(int arg)
{
    switch (arg) {
    case 1: return(":/Images/frais.jpg"); break;
    case 2: return(":/Images/fruit.jpg"); break;
    case 3: return(":/Images/alimentaire.png"); break;
    case 4: return(":/Images/sucrerie.jpg"); break;
    case 5: return(":/Images/surgele.jpg"); break;
    case 6: return(":/Images/hygiene.png"); break;
    case 7: return(":/Images/animals.png"); break;
    case 8: return(":/Images/vetement.png"); break;
    case 10: return(":/Images/boisson.jpg"); break;
    case 11: return(":/Images/alcool.png"); break;
    case 12: return(":/Images/ptitdej.jpg"); break;

    default:return ":/Images/autre.png";
        break;
   }
}
QString MainWindow::getPath3(int arg)
{
    switch (arg) {
    case 1: return(":/Images/carf.gif"); break;
    case 2: return(":/Images/leclerc.jpg"); break;
    case 4: return(":/Images/u.jpg"); break;
    case 3: return(":/Images/aldi.png"); break;
    default:return ":/Images/autre.png";
        break;
    }




}

void MainWindow::on_listWidgetFavoris_itemPressed(QListWidgetItem *item)
{
    currentItem=ui->listWidgetFavoris->currentItem()->text();
    timer6->start(1500);
}

void MainWindow::on_listWidgetFavoris_itemClicked(QListWidgetItem *item)
{
    timer6->stop();

   idNewProd=vecteuFavoris.value(ui->listWidgetFavoris->currentRow());
      DialogQteNewProd AjoutProd(this);
    //on fait passer les informations
    idNewProd=vecteuFavoris.value(ui->listWidgetFavoris->currentRow());
    newProd=true;
    if(ui->listWidgetFavoris->currentItem()->text().at(ui->listWidgetFavoris->currentItem()->text().length()-1)==')')
    {
        newProd=false;
        qteNewProd=ui->listWidgetFavoris->currentItem()->text().at(ui->listWidgetFavoris->currentItem()->text().length()-2);
    }
    AjoutProd.getProd(vecteuFavoris.value(ui->listWidgetFavoris->currentRow()),maListe);
    //on veut placer la fenetre correctement

        QDesktopWidget *desktop2 = QApplication::desktop();
        int screenWidth, width;
        int screenHeight, height;
        int x, y;
        QSize windowSize;
        screenWidth = desktop2->width(); //     obtenir la largeur de l'écran
        screenHeight = desktop2->height(); // obtenir la hauteur de l'écran

       //peu de calculs
        y = screenHeight / 2;


        AjoutProd.move( 35, y);



    AjoutProd.exec();
}

void MainWindow::on_pushButtonOrdreRayon_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(9);
}
void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidgetAppli->setCurrentIndex(3);
}

void MainWindow::on_pushButton_6_clicked()
{
    for(int i=0;i<vecteuOrdreRayon.size();i++)
       {
        QSqlQuery req("update rayon set ordre ="+QString::number(i+1)+" where libelle='"+ui->listWidgetOrderRayon->item(i)->text()+"'");
       }
    ui->stackedWidgetAppli->setCurrentIndex(3);

}
