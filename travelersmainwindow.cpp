#include "travelersmainwindow.h"
#include "ui_travelersmainwindow.h"

TravelersMainWindow::TravelersMainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::TravelersMainWindow)
{
    ui->setupUi(this);



    populateDisplay();


    //        DbManager::getInstance()->getCities();
    //        DbManager::getInstance()->initDataBase();
    //    DbManager::getInstance()->readInTxtFile();

    connect(ui->actionAdmin_Login, SIGNAL(triggered(bool)),this, SLOT(openAdminWindow()));
    startingLocation = "NULL";
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
        QPushButton* cityName = new QPushButton(cities[index]+ "\nDistance to Berlin: " + QString::number(distanceToBerlin) ,this);

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
        QPushButton * item = new QPushButton(menuItems[index].name, this);

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
    qDebug() << "size: " << menuItems.size();

    // for loop - displays the items inside menuItems vector
    for(int index = 0; index < menuItems.size(); index++)
    {
         // creates a new button that will display the name of the item
        QPushButton * item = new QPushButton(menuItems[index].name, this);

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
    qDebug() << "hello";

    //adminWindow->close();
    //show();
}

void TravelersMainWindow::openTripOperationsWindow()
{
    hide();
    tripOperations = new Trip(this);
    tripOperations->show();
}












void TravelersMainWindow::on_makeCustomTripButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2); // make custom trip index is '2'
    ui->checkCitiesWidget->clear();
    ui->makeCustomTripButtonPages->setCurrentIndex(0);
    startingLocation = "NULL"; // DEBUG(?) RESET SELECTED INITIAL CITY

    QSqlQuery query;
    QVector<QString> cityNames = {"Amsterdam"};
    query.prepare("SELECT Ending from Distances WHERE Starting=\"Amsterdam\"");
    query.exec();

    while (query.next())
    {
        if (!vectorContains(cityNames, query.value(0).toString()))
            cityNames.append(query.value(0).toString());
    }

    for (int i = 0; i < cityNames.size(); i++)
    {
        QListWidgetItem *listItem = new QListWidgetItem(cityNames[i]);
        listItem->setCheckState(Qt::Unchecked);
        ui->checkCitiesWidget->addItem(listItem);
    }

    ui->makeCustomTripButtonPages->setCurrentIndex(1);
}

// checks to see whether or not a vector contains a given element
bool TravelersMainWindow::vectorContains(const QVector<QString> &elements, const QString name)
{
    for (int i = 0; i < elements.size(); i++)
    {
        if (elements[i] == name)
            return true;
    }

    return false;
}

// gets the 'checked' cities when a user selects the custom trip option
QVector<QString> TravelersMainWindow::getSelectedCities(QListWidget *widget, bool initialCityOnly)
{
    QVector<QString> selectedCities;

    if (initialCityOnly)
    {
        QStringList checkedCities;

        for (int i = 0; i < widget->model()->rowCount(); i++)
        {
            if (widget->item(i)->checkState())
                checkedCities.append(widget->item(i)->text());
        }

        if (checkedCities.size() == 1)
            startingLocation = checkedCities[0];

        selectedCities.clear();

        return selectedCities;
    }

    else
    {
        for (int i = 0; i < widget->model()->rowCount(); i++)
        {
            if (widget->item(i)->checkState())
                selectedCities.push_back(widget->item(i)->text());
        }

        return selectedCities;
    }
}


QVector<City> TravelersMainWindow::modifiedNextClosest(QVector<City> cities, QVector<QString> selectedCities, QString startingCity)
{
    City potentialLocation = City();	// City variable that holds the data of the city that is potentially closest
    QString currentCity = "";	// A QString to tell the database what city we're currently on
    QSqlQuery query;	// The variable we're accessing the database with

    // If startingCity has a value, use that city to query the database with
    if ((startingCity != "") && (cities.size() == 0))
    {
        currentCity = startingCity;
        cities.push_back(City(startingCity, 0));
    }

    // If there is not a startingCity parameter, use the last city we visited (from our QVector)
    else
    {
        // Failure to pass an initial city (error check)
        if (cities.size() == 0)
            return cities;

        currentCity = cities.at(cities.size() - 1).getName();
    }

    // Request data from the database based on the current city
    query.prepare("SELECT Ending, Distance from Distances WHERE Starting=(:val1)");
    query.bindValue(":val1", currentCity);
    query.exec();

    // While there are still elements in the query
    while (query.next())
    {
        // If the current city is NOT a visited city, continue, else, ignore the current query
        // Additionally, check to see if the city is one of the cities that the user selected
        //      if it is, continue, else, ignore the current query
        if ((!contains(cities, query.value(0).toString())) && (vectorContains(selectedCities, query.value(0).toString())))
        {
            // If this is the first valid city, set it as our closest city
            if (potentialLocation.getName() == "NULL")
            {
                potentialLocation.setName(query.value(0).toString());
                potentialLocation.setDistance(query.value(1).toInt());
            }

            else
            {
                // If it is not the first city, check to see if the distance is shorter than our current closest
                // If it is closer, make it our closest city
                if (query.value(1).toInt() < potentialLocation.getDistance())
                {
                    potentialLocation.setName(query.value(0).toString());
                    potentialLocation.setDistance(query.value(1).toInt());
                }
            }
        }
    }

    // If our query returned no results, return cities (error check)
    if (potentialLocation.getName() == "NULL")
        return cities;

    // Once we've checked all the cities at a given location, add the closest city
    cities.push_back(potentialLocation);

    // If our cities QVector size is equal to or greater than the number of cities we want to visit, return it
    // (Greater than is an error check)
    if (cities.size() >= selectedCities.size() + 1)
        return cities;

    // Else, continue calling the algorithm
    else
        return modifiedNextClosest(cities, selectedCities);
}

// Helper function for our Algorithm
// Checks to see whether or not we've visited a city before
bool TravelersMainWindow::contains(const QVector<City> &cities, const QString location)
{
    for (int i = 0; i < cities.size(); i++)
    {
        if (cities[i].getName() == location)
            return true;
    }

    return false;
}

void TravelersMainWindow::on_pushErrorButton_clicked()
{
    on_makeCustomTripButton_clicked();
}

void TravelersMainWindow::on_makeCustomTripButtonConfirm_clicked()
{
    getSelectedCities(ui->checkCitiesWidget, true);

    if (startingLocation == "NULL")
        ui->stackedWidget->setCurrentIndex(3);

    else
        populateSubsequentCustomTripOptions();
}

void TravelersMainWindow::populateSubsequentCustomTripOptions()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->makeCustomSubTripButtonPages->setCurrentIndex(0);
    ui->checkSubsequentCities->clear();
    QSqlQuery query;
    QVector<QString> cityNames;
    query.prepare("SELECT Ending from Distances WHERE Starting=(:val1)");
    query.bindValue(":val1", startingLocation);
    query.exec();

    while (query.next())
    {
        if (!vectorContains(cityNames, query.value(0).toString()))
            cityNames.append(query.value(0).toString());
    }

    for (int i = 0; i < cityNames.size(); i++)
    {
        QListWidgetItem *listItem = new QListWidgetItem(cityNames[i]);
        listItem->setCheckState(Qt::Unchecked);
        ui->checkSubsequentCities->addItem(listItem);
    }

    ui->makeCustomSubTripButtonPages->setCurrentIndex(1);
}

void TravelersMainWindow::on_makeCustomSubTripButtonConfirm_clicked()
{
    QVector<QString> subsequentCities = getSelectedCities(ui->checkSubsequentCities);

    if (subsequentCities.size() < 1)
        ui->stackedWidget->setCurrentIndex(5);

    else
    {
        QVector<City> customTrip;
        customTrip = modifiedNextClosest(customTrip, subsequentCities, startingLocation);

        ui->completedCustomTrip->clear();

        for (int i = 0; i < customTrip.size(); i++)
            ui->completedCustomTrip->addItem(QString::number(i + 1) + ": " + customTrip[i].getName());

        ui->stackedWidget->setCurrentIndex(6);
    }

}

void TravelersMainWindow::on_subsequentTripErrorButton_clicked()
{
    populateSubsequentCustomTripOptions();
}
