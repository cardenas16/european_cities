#include "admin.h"
#include "ui_admin.h"

#include <QSqlRecord>
#include <QAbstractItemModel>
#include <QSqlField>
#include <QMessageBox>
#include <QAbstractItemView>

Admin::Admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
    inputRuns = 0;
    repeatedItem = false;




    tableModel = new QSqlTableModel(this);
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tableModel->setTable("Items");
    tableModel->select();




//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    tableModel->setHeaderData(0, Qt::Horizontal, "City");
    tableModel->setHeaderData(1, Qt::Horizontal, "Item");
    tableModel->setHeaderData(2, Qt::Horizontal, "Price");
    tableModel->setHeaderData(3, Qt::Horizontal, "Purchase Count");
    tableModel->setHeaderData(4, Qt::Horizontal, "Purchase Amount Total");
//    tableModel->flags(QModelIndex()).setFlag(Qt::ItemIsSelectable);


    tableModel->select();
    ui->tableView->setModel(tableModel);




    ui->tableView->setFocusPolicy(Qt::NoFocus);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);




    connect(ui->logOutButton,SIGNAL(clicked()),this->parent(),SLOT(on_pushButton_3_clicked()));


//    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(on_saveButton_clicked()));

    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(test(QModelIndex)));

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)),this, SLOT(selectedRow(QModelIndex)));










}
void Admin::onTableClicked(const QSqlRecord &index)
{
    qDebug() << "inside";

    index.indexOf("City");

}

Admin::~Admin()
{
    delete ui;
}



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
    ui->stackedWidget_2->setCurrentIndex(1);
    QVector<QString> cities = DbManager::getInstance()->getCities();
    QString selectedCity = cities.at(index);

    tableModel->setFilter("City='"+selectedCity+"'");

    tableModel->setHeaderData(1, Qt::Horizontal, "Item");

    tableModel->setHeaderData(2, Qt::Horizontal, "Price");
    tableModel->setHeaderData(3, Qt::Horizontal, "Purchase Count");
    tableModel->setHeaderData(4, Qt::Horizontal, "Purchase Amount Total");
    tableModel->select();


    ui->tableView->setModel(tableModel);
        ui->tableView->hideColumn(0);

}



void Admin::on_addItemButton_clicked()
{

// ui->tableView->setEditTriggers(QAbstractItemView::EditTriggers(Qt::ItemIsEditable));

    // sets the number of rows
    int row = tableModel->rowCount(QModelIndex());


    QString item;



    QVector<QString> cities = DbManager::getInstance()->getCities();
    int index = ui->comboBox->currentIndex();
    QString selectedCity = cities.at(index);


    bool emptyCell = false;
    for(int index = 0; index < 3; index++)
    {
        if(tableModel->record(row-1).isNull(index))
        {
            emptyCell = true;
        }
    }

    if(inputRuns > 0)
    {
        qDebug() << "record: " << (tableModel->record(row-1).value(1)).toString();
        item = (tableModel->record(row-1).value(1)).toString();
        if(DbManager::getInstance()->itemExist(selectedCity,item))
        {
            qDebug() << "repeated item";
            repeatedItem = true;
        }
    }


    if(emptyCell || repeatedItem)
    {
        qDebug() << "theres an empty cell";
        repeatedItem = false;
    }
    else
    {
        tableModel->submitAll();


        tableModel->insertRow(tableModel->rowCount(QModelIndex()));
        QSqlRecord myRecord = tableModel->record();
        myRecord.setValue(0,selectedCity);
        tableModel->setRecord(row, myRecord);

    }
    inputRuns++;




}


void Admin::test(QModelIndex index)
{

    if(index.column() == 1)
    {
        int comboIndex = ui->comboBox->currentIndex();

        QString item = (index.data()).toString();
        QVector<QString> cities = DbManager::getInstance()->getCities();
        QString selectedCity = cities.at(comboIndex);

        if(DbManager::getInstance()->itemExist(selectedCity, item))
        {
            qDebug() << "error: item exist\n";


            QMessageBox::warning(this, tr("My Application"),
                                           tr("This item already exists.\n"
                                              "The cell will be cleared."), QMessageBox::Ok );
            qDebug() << tableModel->rowCount(QModelIndex());
            tableModel->setData(index," ",Qt::EditRole);

        }
        else
        {
            qDebug() << "new original item\n";

        }
        //    qDebug() << "column: " << (index.data()).toString();




    }




}



void Admin::selectedRow(QModelIndex index)
{
//    qDebug() << "row: " << index.row() << " col: " << index.column();

    qDebug() << ( index.data()).toString();
    highlightedCell = index;

//    connect(ui->deleteButton, SIGNAL(clicked()),this,SLOT(eliminate(index)));

}




void Admin::on_deleteButton_clicked()
{
     QString item = (highlightedCell.data()).toString();
     int comboIndex = ui->comboBox->currentIndex();
     QVector<QString> cities = DbManager::getInstance()->getCities();
     QString selectedCity = cities.at(comboIndex);

     qDebug() << "selected city: " << selectedCity << " item: " << item;
     //DbManager::getInstance()->deleteItem(selectedCity, item);

    setTableView(comboIndex);
}
