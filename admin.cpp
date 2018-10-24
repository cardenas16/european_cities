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

    // initializes the vector of cities, with the current cities inside the database
    cities = DbManager::getInstance()->getCities();


    // sets the stackedWidget to the first page. (The login page)
    ui->stackedWidget->setCurrentIndex(0);

    // pattern matching for new city & and new item
    QRegExp rx("^([A-Z]\\d{0}[a-z]*-?[a-z]* )+([A-Z]\\d{0}[a-z]*) *$" );

    // pattern mathching for new price
    QRegExp newPriceExp("^[0-9]{1,2}[.][0-9]{1,2}$");

    // validators for lineedits
    QValidator *validator = new QRegExpValidator(rx, this);
    QValidator *priceValidator = new QRegExpValidator(newPriceExp, this);

    // validators are set
    ui->lineEdit_newCity->setValidator(validator);
    ui->lineEdit_item->setValidator(validator);
    ui->lineEdit_price->setValidator(priceValidator);



    // initialized to false, because no row is add
    newRow = false;

    // spinBox delegate is initialized
    newPriceDelegate = new SpinBoxDelegate(this);

    // newItem delegate is initialized
    newItemDelegate   = new NewItemDelegate(this);

    // model for the table is intitalized
    tableModel = new QSqlTableModel(this);

    // mode for holding the city names to display in the comboBox
     citiesModel = new QStringListModel(this);



    // the display for the table is initialized
    initialTableDisplay();


    // sets the delegate for column 2 (price column)
    ui->tableView->setItemDelegateForColumn(2,newPriceDelegate );

    // sets the delegate for column 1 (item column)
    ui->tableView->setItemDelegateForColumn(1,newItemDelegate);




    connect(ui->pb_back,SIGNAL(clicked()),this->parent(),SLOT(adminLoggedOut()));

     connect(ui->logOutButton,SIGNAL(clicked()),this->parent(),SLOT(adminLoggedOut()));

    // once the data is changed in the new item cell, it will call the fuction to validate if the new item is not in the db
    connect(tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(newItemValidation(QModelIndex)));

    // once a new city is choosen, it saves the changes the user made. Like create a new item and not save it
    connect(ui->cityListComboBox, SIGNAL(activated(int)), this, SLOT(saveChanges()));

    // once a comboxBox index is activated it will call setTableView to populate the rows and columns with the indices corresponding items and prices
//    connect(ui->comboBox, SIGNAL(activated(int)),this, SLOT(setTableView(int)));





}


Admin::~Admin()
{
    delete ui;
}


void Admin::initialTableDisplay()
{

    // specifies the table that the model should hold data for
    tableModel->setTable("Items");

    // allows for the data to be inserted into the model
    tableModel->select();

    // the model is set to the tableview to be displayed
    ui->tableView->setModel(tableModel);

    // specifies the cells not to be editable
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // specifies that any change to a cell must by manually commited, that
    // changes to a cell arent return to the database without manually checking if the data is correct
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // sets the headers for tableview
    tableModel->setHeaderData(0, Qt::Horizontal, "City");
    tableModel->setHeaderData(1, Qt::Horizontal, "Item");
    tableModel->setHeaderData(2, Qt::Horizontal, "Price");
//    tableModel->setHeaderData(2, Qt::Horizontal, "Purchase Amount Total");

    ui->tableView->hideColumn(3);
     ui->tableView->hideColumn(4);

    // allows for the horizontal and vertical headers to be strechted out to fit the size of the table
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


}


void Admin::on_loginButton_clicked()
{



    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    bool validAdmin = true;//DbManager::getInstance()->validateAdmin(username, password);
    if(validAdmin)
    {
         ui->stackedWidget->setCurrentIndex(1);

         for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++ )
         {
             citiesList << *it;
         }

         citiesModel->setStringList(citiesList);


         ui->cityListComboBox->setModel(citiesModel);
    }
    else
    {
        QMessageBox::critical(this, tr("My Application"),tr("Invalid User."), QMessageBox::Ok );
    }


}

void Admin::setTableView(int cityIndex)
{

    // sets the newItemStackWidget to the next pages to show the buttons for adding and deleting an item
    ui->newItemStackedWidget->setCurrentIndex(1);

    // the city choosen by clicking the comboBox is initialized
    QString selectedCity = cities.at(cityIndex);

    // filters the "Items" table to only showing the items that correlate with the selectedCity
    tableModel->setFilter("City='"+selectedCity+"'");


    // sets the headers for tableview
    tableModel->setHeaderData(0, Qt::Horizontal, "City");
    tableModel->setHeaderData(1, Qt::Horizontal, "Item");
    tableModel->setHeaderData(2, Qt::Horizontal, "Price");
//    tableModel->setHeaderData(3, Qt::Horizontal, "Purchase Count");
//    tableModel->setHeaderData(4, Qt::Horizontal, "Purchase Amount Total");

    // allows for the data to be inserted into the model
    tableModel->select();


    // the model is set to the tableview to be displayed
    ui->tableView->setModel(tableModel);

        ui->tableView->hideColumn(0);
         ui->tableView->hideColumn(3);
          ui->tableView->hideColumn(4);


    // sends a signal when an item has been clicked on, it passes the location of where the item is located in the tableview
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(cellClicked(const QModelIndex &)));

}


// only called a cell is clicked on in the table
void Admin::cellClicked(const QModelIndex &index)
{
    // assigned the cell that is clicked on
    highlightedCell = index;

    // checks to see if the cell highlighted is the price cell. if so it is allowed to be editable
    if(highlightedCell.column() == 2)
    {
        ui->tableView->edit(highlightedCell);

    }

    // checks to see if there has been a new row inserted and if the highlighted cell is the last row inside the table.
    // Therefore only allowing a item cell to be editable if there has been a new row added.
    if(highlightedCell.row() == tableModel->rowCount(QModelIndex())-1 && newRow == true)
    {
        ui->tableView->edit(highlightedCell);
    }

}

// validates the cell of a cell that has been changed
void Admin::newItemValidation(QModelIndex index)
{

    // checks if the data changed was for the item cell
    if(index.column() == 1)
    {
        // a int variable is created to hold the index of the selected index of the comboBox

        // a string varible is created to be initialized to hold the name of choosen city selected by the comboBox index
        QString selectedCity = cities.at(comboBoxIndex);

        // a string varibale is created to hold the name of the newly changed item
        QString item = (index.data()).toString();

        // the item is trimmed to cut off any white spaces left by the user
        item = item.trimmed();

        //        QVector<QString> cities = DbManager::getInstance()->getCities();


        // calls the db manager to check if the newly inserted item already exists inside the database
        // if so the item cell is cleared for the user, and ready for another item string
        if(DbManager::getInstance()->itemExist(selectedCity, item))
        {
            QMessageBox::warning(this, tr("My Application"),
                                 tr("This item already exists.\n"
                                    "The cell will be cleared."), QMessageBox::Ok );
            tableModel->setData(index," ",Qt::EditRole);

        }

    }

}

// allows for a new item to be inserted into the database
void Admin::on_button_addItem_clicked()
{

    // a int variable is created to hold the last row number in the table
    int row = tableModel->rowCount(QModelIndex())-1;

    // a int variable is created to hold the first column number in the table
    int col = 0;

    // a modelIndex is initialized to the first cell inside in the last row for error checking
    QModelIndex newRowIndex = tableModel->index(row, col, QModelIndex());

    // boolean variable instantiated to false
    bool emptyCell = false;

    // for loop - loops through the last row, from the first column to the price column
    //            checking if any of the cells in the newly created row is emoty
    //            if so, the emptyCell varible is assigned true. Otherwise false
    for(int index = 0; index < 3; index++)
    {
        // index of the cell is assigned to the next column
        newRowIndex = tableModel->index(row, index, QModelIndex());

        // checks if the cell is empty
        if((tableModel->data(newRowIndex)).toString() == "")
        {
            emptyCell = true;
        }

    }


    // if - allows for a new row to be inserted only if there are no emptyCells on the previous row
    if(emptyCell != true)
    {

        // manually submits the item and price currently in the row.
        tableModel->submitAll();

        // inserts a new row inside the tableModel for new item and price to be entered
        tableModel->insertRow(tableModel->rowCount(QModelIndex()));

        // allows for the vertical headers to be strechted out to fit the size of the table
        ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // newRow is set to true, because there has been a new row inserted
        newRow = true;

        // last row number in the table in assigned
        row = tableModel->rowCount(QModelIndex())-1;


        //        QVector<QString> cities = DbManager::getInstance()->getCities();


        QString selectedCity = cities.at(comboBoxIndex);

        // record is made to populate the first column of the newly created row with the name of
        // the city getting the a new item
        QSqlRecord myRecord = tableModel->record();

        // record is assigned to the first column
        myRecord.setValue(0,selectedCity);

        // record is inputted into the tableModel
        tableModel->setRecord(row, myRecord);


        // a modelIndex is created to hold the data of the model
        QModelIndex index = tableModel->index(row,1,QModelIndex());

        // the index is then passed to the function so the cell can be editted
        cellClicked(index);

    }
    else
    {
        QMessageBox::warning(this, tr("My Application"), tr("There is an Empty Cell."), QMessageBox::Ok );
    }

}



// needs editing. needs to save the
void Admin::saveChanges()
{


    comboBoxIndex = ui->cityListComboBox->currentIndex();
    newRow = false;

    // a int variable is created to hold the last row number in the table
    int row = tableModel->rowCount(QModelIndex())-1;

    // a int variable is created to hold the first column number in the table
    int col = 0;

    // a modelIndex is initialized to the first cell inside in the last row for error checking
    QModelIndex newRowIndex = tableModel->index(row, col, QModelIndex());


    bool emptyCell = false;

    // for loop - loops through the last row, from the first column to the price column
    //            checking if any of the cells in the newly created row is emoty
    //            if so, the emptyCell varible is assigned true. Otherwise false
    for(int index = 0; index < 3; index++)
    {
        // index of the cell is assigned to the next column
        newRowIndex = tableModel->index(row, index, QModelIndex());

        // checks if the cell is empty
        if((tableModel->data(newRowIndex)).toString() == "")
        {
            emptyCell = true;
        }

    }

    // if theres an empty cell, a message is sent to the admin that changes werent commited
    if(emptyCell == true)
    {
        QMessageBox::warning(this, tr("My Application"), tr("Items were not saved, Empty Cells."), QMessageBox::Ok );
    }
    else
    {
        tableModel->submitAll();

    }

    // call setTableView to populate the rows and columns with the indices corresponding items and prices
    setTableView(comboBoxIndex);

}






void Admin::on_button_deleteItem_clicked()
{
    // a variable string is created to hold the item clicked on for deletion
    QString item = (highlightedCell.data()).toString();


    QString selectedCity = cities.at(comboBoxIndex);

    qDebug() << "selected city: " << selectedCity << " item: " << item;
    DbManager::getInstance()->deleteItem(selectedCity, item);

    // call setTableView to populate the rows and columns with the indices corresponding items and prices
    setTableView(comboBoxIndex);
}


// changes the stackedWidget to the third page so a new and its items can be added
void Admin::on_newCityButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// allows for the admin to return to the viewing page of the items and prices of a city
void Admin::on_backButton_newCity_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    // cities
    QStringList citiesList;

    cities = DbManager::getInstance()->getCities();

    for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++ )
    {
        citiesList << *it;
    }

    citiesModel->setStringList(citiesList);



    ui->cityListComboBox->setModel(citiesModel);
}


void Admin::on_saveButton_newCity_clicked()
{
    QString newCity = ui->lineEdit_newCity->text();
    QString item = ui->lineEdit_item->text();
    double  price = (ui->lineEdit_price->text()).toDouble();

    newCity = newCity.trimmed();
    item = item.trimmed();

    bool originalItemExists = DbManager::getInstance()->itemExist(newCity, item);

    if(originalItemExists == true)
    {
        QMessageBox::warning(this, tr("My Application"),
                             tr("This item already exists.\n"
                                "The cell will be cleared."), QMessageBox::Ok );
        ui->lineEdit_item->clear();
        return;
    }




    bool emptyLine = newCity.isEmpty() || item.isEmpty() || ui->lineEdit_price->text().isEmpty();


    if(emptyLine != true)
    {

        QMessageBox::information(this, tr("My Application"),
                                 tr("Item added."), QMessageBox::Ok );


        ui->lineEdit_price->clear();
        ui->lineEdit_item->clear();
        DbManager::getInstance()->addItem(newCity, item, price);
        DbManager::getInstance()->addEuropeanCity(newCity);



    }
    else
    {
        qDebug() << "cannot add, theres an empty line";

        QMessageBox::warning(this, tr("My Application"),
                             tr("Cannot Add new item.\n"
                                "Theres an empty line"), QMessageBox::Ok );
    }

    cities = DbManager::getInstance()->getCities();



    qDebug() << "newCity: " << newCity << endl
             << "item: " << item << endl
             << "price: " << price;
}



void Admin::on_button_importCities_clicked()
{
    ui->importCitiesStackWidget->setCurrentIndex(1);
    DbManager::getInstance()->readNewCitiesTxtFile();

    cities.clear();
    citiesList.clear();

    cities = DbManager::getInstance()->getCities();


    for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++ )
    {
        citiesList << *it;
    }

    citiesModel->setStringList(citiesList);



    ui->cityListComboBox->setModel(citiesModel);
}

