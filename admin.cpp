#include "admin.h"
#include "ui_admin.h"

Admin::Admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);

    tableModel = new QSqlTableModel(this);
    tableModel->setTable("Items");
    tableModel->select();

    ui->tableView->setModel(tableModel);
    connect(ui->logOutButton,SIGNAL(clicked()),this->parent(),SLOT(on_pushButton_3_clicked()));
}

Admin::~Admin()
{

    //               connect(ui->actionAdmin_Login, SIGNAL(triggered(bool)),this, SLOT(openAdminWindow()));
    delete ui;
}

/*
 *
AdminOperations::AdminOperations(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminOperations)
{
    ui->setupUi(this);

    ui->tableView->setEditTriggers(QAbstractItemView::EditKeyPressed);



}

AdminOperations::~AdminOperations()
{
    delete ui;
}

void AdminOperations::on_loginButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    citiesModel = new QStringListModel(this);

    QStringList citiesList;

    QVector<QString> cities = DbManager::getInstance()->getCities();

    for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++ )
    {
        citiesList << *it;
    }

    citiesModel->setStringList(citiesList);



    ui->comboBox->setModel(citiesModel);



     connect(ui->comboBox, SIGNAL(activated(int)),this, SLOT(setTableView(int)));



}

void AdminOperations::setTableView(int index)
{
    QVector<QString> cities = DbManager::getInstance()->getCities();
    queryModel = new QSqlQueryModel(this);


////         int index = ui->comboBox->currentIndex();
//    QString selectedCity = cities.at(index);

//    queryModel->setQuery("SELECT Item, Cost, PurchaseCount, PurchaseAmtTotal FROM Items WHERE City='"+selectedCity+"'");
//    queryModel->setHeaderData(0, Qt::Horizontal, tr("Item"));
//    queryModel->setHeaderData(1, Qt::Horizontal, tr("Cost"));
//    queryModel->setHeaderData(1, Qt::Horizontal, tr("Amount of Purchase"));
//    queryModel->setHeaderData(1, Qt::Horizontal, tr("Total Purchase"));

//    ui->tableView->setModel(queryModel);


    model = new QSqlTableModel(this);
    model->setTable("Items");
    model->select();

     ui->tableView->setModel(model);


}

void AdminOperations::on_addItemButton_clicked()
{
    ui->newItemStackedWidget->setCurrentIndex(1);

}

void AdminOperations::on_saveButton_clicked()
{
    QVector<QString> cities = DbManager::getInstance()->getCities();

    int index = ui->comboBox->currentIndex();

    QString city = cities.at(index);

    QString item = ui->itemLineEdit->text();

    double price = (ui->priceLineEdit->text()).toDouble();

    qDebug() << "city: " << city << "item: " << item << "price: " << price;


    if(DbManager::getInstance()->itemExist(city, item))
    {
        qDebug() << "item exist";
    }
    else
    {
        qDebug() << "item DOES NOT exist";
    }
*/



void Admin::on_loginButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    citiesModel = new QStringListModel(this);

    QStringList citiesList;

    QVector<QString> cities = DbManager::getInstance()->getCities();

    for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++ )
    {
        citiesList << *it;
    }

    citiesModel->setStringList(citiesList);



    ui->comboBox->setModel(citiesModel);



    connect(ui->comboBox, SIGNAL(activated(int)),this, SLOT(setTableView(int)));

}

void Admin::setTableView(int index)
{

    QVector<QString> cities = DbManager::getInstance()->getCities();
    QString selectedCity = cities.at(index);

    tableModel->setFilter("City='"+selectedCity+"'");


    tableModel->setHeaderData(0, Qt::Horizontal, "Item");
    tableModel->setHeaderData(1, Qt::Horizontal, "Price");
    tableModel->setHeaderData(2, Qt::Horizontal, "Purchase Count");
    tableModel->setHeaderData(2, Qt::Horizontal, "Purchase Amount Total");
    tableModel->select();


    ui->tableView->setModel(tableModel);
    ui->tableView->hideColumn(0);

}


