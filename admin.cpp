#include "admin.h"
#include "ui_admin.h"

#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QAbstractItemDelegate>
#include <QItemDelegate>


Admin::Admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);

    newRow = false;






    mySpinBox = new SpinBox(this);



    tableModel = new QSqlTableModel(this);

    myDelegate   = new customDelegate(this);


    tableModel->setTable("Items");
    tableModel->select();

    ui->tableView->setModel(tableModel);
    ui->tableView->setItemDelegateForColumn(2,mySpinBox );

    ui->tableView->setItemDelegateForColumn(1,myDelegate);


    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    tableModel->setHeaderData(0, Qt::Horizontal, "Item");
    tableModel->setHeaderData(1, Qt::Horizontal, "Price");
    tableModel->setHeaderData(2, Qt::Horizontal, "Purchase Count");
    tableModel->setHeaderData(2, Qt::Horizontal, "Purchase Amount Total");


    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);




    connect(ui->logOutButton,SIGNAL(clicked()),this->parent(),SLOT(on_pushButton_3_clicked()));



    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(test(QModelIndex)));

    // resets the boolean to false, so that that last item on the table can not be edited.
    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(resetNewRow()));



}


Admin::~Admin()
{

    //               connect(ui->actionAdmin_Login, SIGNAL(triggered(bool)),this, SLOT(openAdminWindow()));
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

void Admin::setTableView(int cityIndex)
{



    ui->newItemStackedWidget->setCurrentIndex(1);
    QVector<QString> cities = DbManager::getInstance()->getCities();
    QString selectedCity = cities.at(cityIndex);



    tableModel->setFilter("City='"+selectedCity+"'");


    tableModel->setHeaderData(0, Qt::Horizontal, "City");
    tableModel->setHeaderData(1, Qt::Horizontal, "Item");
    tableModel->setHeaderData(2, Qt::Horizontal, "Price");
    tableModel->setHeaderData(3, Qt::Horizontal, "Purchase Count");
    tableModel->setHeaderData(4, Qt::Horizontal, "Purchase Amount Total");
    tableModel->select();



    ui->tableView->setModel(tableModel);

    //    ui->tableView->hideColumn(0);



    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));




}

void Admin::onTableClicked(const QModelIndex &index)
{

    if(index.column() == 2)
    {
        ui->tableView->edit(index);

    }

    if(index.row() == tableModel->rowCount(QModelIndex())-1 && newRow == true)
    {
        ui->tableView->edit(index);
    }




}


void Admin::test(QModelIndex index)
{


    if(index.column() == 1)
    {
        int comboIndex = ui->comboBox->currentIndex();


        QString item = (index.data()).toString();
        item = item.trimmed();
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

    }

}


void Admin::on_addItemButton_clicked()
{
//    newRow = false;




    // rowCount ouptuts the row count, its starts at 0
    int row = tableModel->rowCount(QModelIndex())-1;
    qDebug() << "row: "<< row << " "<<  tableModel->record(row) << endl;
    int col = 0;
    QModelIndex p = tableModel->index(row, col, QModelIndex());


    bool emptyCell = false;
    for(int index = 0; index < 3; index++)
    {
        p = tableModel->index(row, index, QModelIndex());

        if((tableModel->data(p)).toString() == "")
        {
            emptyCell = true;
        }

    }



    if(emptyCell != true)
    {


        tableModel->submitAll();

        ui->newItemStackedWidget->setCurrentIndex(1);
        tableModel->insertRow(tableModel->rowCount(QModelIndex()));



        ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);



        newRow = true;

        row = tableModel->rowCount(QModelIndex())-1;


        QVector<QString> cities = DbManager::getInstance()->getCities();

        int comboBoxIndex = ui->comboBox->currentIndex();

        QString selectedCity = cities.at(comboBoxIndex);

        QSqlRecord myRecord = tableModel->record();

        myRecord.setValue(0,selectedCity);

        tableModel->setRecord(row, myRecord);



        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


        QModelIndex index = tableModel->index(row,1,QModelIndex());

        onTableClicked(index);




    }
    else
    {
        QMessageBox::warning(this, tr("My Application"),
                             tr("There is an Empty Cell."), QMessageBox::Ok );

    }



}



void Admin::resetNewRow()
{
    newRow = false;
}
