#include "travelersmainwindow.h"
#include "ui_travelersmainwindow.h"

#include <QStringList>
#include <QLineEdit>
#include <iostream>



TravelersMainWindow::TravelersMainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::TravelersMainWindow)
{
    ui->setupUi(this);


//        DbManager::getInstance()->initDataBase();
    populateDisplay();


    //        DbManager::getInstance()->getCities();

    //    DbManager::getInstance()->readInTxtFile();

    connect(ui->actionAdmin_Login, SIGNAL(triggered(bool)),this, SLOT(openAdminWindow()));

}


TravelersMainWindow::~TravelersMainWindow()
{
    delete ui;
}


// populates the display in the travleres main window
void TravelersMainWindow::populateDisplay()
{
    // string list that will hold the cities that are display in the main menu
    QStringList euroCities;


    // vecotor of european cities is initialized by the get cities from db manager
    QVector<QString> cities = DbManager::getInstance()->getCities();


    int row = 0;
    int col = 0;

    for(int index = 0; index < cities.size(); index++)
    {
        // a call to the db manager to get the distance betweem two cities, passes the starting ctiy name and then the ending city name
        float distanceToBerlin = DbManager::getInstance()->getDistanceInbetween(cities[index], "Berlin");



        // creates a new push button, the button is displayed with the name of the city and the distance to berlin
        QPushButton* cityName = new QPushButton(cities[index]+ "\nDistance to Berlin: "  +  QString::number(distanceToBerlin) ,this);

        // sets the name of the object, this will be used as a parameter in the next line
        cityName->setObjectName(cities[index]);

        // passes the name of the city, SIGNAL-> once clicked, passes this, then passes the function to call once clicked
        connect(cityName, SIGNAL(clicked()), this, SLOT(selectedCity()));


        cityName->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        // addeds the widget to main window, by name, row, and col. COl is always set to zero so it shows it vertically
        ui->gridLayout_cities->addWidget(cityName, row, col);

        // spaces out the buttons
        ui->gridLayout_cities->setSpacing(5);

        // incremens the row count
        row++;

    }



}


// SLOT - this function will be called when the traveler presses a button of a certain city. It will move to index 1 of the stacked widget
//        to display the current items the city holds
void TravelersMainWindow::selectedCity()
{
    // sets a new index for the stacked widget
    ui->stackedWidget->setCurrentIndex(1);

    // Get the QPushButton object that was clicked
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());


    // acces the name of the city that had been clicked on
    QString name = clickedButton->objectName();

    // get the distance between the clicked city, and berlin
    float distance = DbManager::getInstance()->getDistanceInbetween(name, "Berlin");

    qDebug() << "the distance is: " << distance;

    int col = 0;

    // displays the distance to the distanceLabel
    ui->distanceLabel->setText(QString::number(distance));

    // displays the city of where the traditional items are from
    ui->cityLabel->setText(name);

    qDebug() << "the name of the clicked object is: " << name << endl;

    // gets the meny items for the specified city by called the db manager
    QVector<TraditionalFoodItems> menuItems = DbManager::getInstance()->getMenuItems(name);


    qDebug() << "size: " << menuItems.size();


    // for loop - displays the items inside menuItems vector
    for(int index = 0; index < menuItems.size(); index++)
    {

        // creates a new button that will display the name of the item
        QPushButton * item = new QPushButton(menuItems[index].name + "\n$" + QString::number(menuItems[index].price), this);

        item->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        // adds the widget to the gridlayout
        ui->gridLayout_items->addWidget(item, index, col);

    }


}


// displays the item of the next city
void TravelersMainWindow::on_pb_NextCity_clicked()
{
    // returns the cities from the db manager
    QVector<QString> cities = DbManager::getInstance()->getCities();

    // the previous city is initialized from the cityLable
    QString previousCity = ui->cityLabel->text();

    // removes the items displayed from the previous city before populating new item of the new city
    while( ui->gridLayout_items->count() )
    {
        QWidget* widget = ui->gridLayout_items->itemAt(0)->widget();
        if( widget ) {
            ui->gridLayout_items->removeWidget(widget);

            delete widget;
        }
    }

    int indexFound = 0;
    int newIndex = 0;
    int col = 0;

    // searches through the cities vector to locate the index of the previous city
    for(int index = 0; index < cities.size(); index++)
    {
        if(previousCity == cities.at(index))
            indexFound = index; // if the city is found, the indexFound is set to the index of where it was found
    }

    newIndex = (indexFound +1)%cities.size(); // increments the indeFound by one, then assigns it to newIndex (the index of the next city)

    // stores the name of the new city
    QString cityName = cities[newIndex];

    // a call to the db manager to get the distance betweem two cities, passes the starting ctiy name and then the ending city name
    float distanceToBerlin = DbManager::getInstance()->getDistanceInbetween(cityName, "Berlin");

    qDebug() << "the city is: " << cityName;

    //  calles db manager to obtain the items for the certain city
    QVector<TraditionalFoodItems> menuItems = DbManager::getInstance()->getMenuItems(cityName);



    // for loop - displays the items inside menuItems vector
    for(int index = 0; index < menuItems.size(); index++)
    {
        // creates a new button that will display the name of the item
        QPushButton * item = new QPushButton(menuItems[index].name + "\n$" + QString::number(menuItems[index].price), this);

        item->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        // adds the widget to the gridlayout
        ui->gridLayout_items->addWidget(item, index, col);
    }

    // sets the city labal, with the current city
    ui->cityLabel->setText(cityName);
    ui->distanceLabel->setText(QString::number(distanceToBerlin));


}

void TravelersMainWindow::openAdminWindow()
{

    hide();
    adminWindow = new Admin(this);
    adminWindow->show();


}

// returns to index zero of the stacked widget
void TravelersMainWindow::on_pb_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void TravelersMainWindow::on_pushButton_3_clicked()
{
    qDebug() << "admin has logged out";
    adminWindow->close();
    show();




}

