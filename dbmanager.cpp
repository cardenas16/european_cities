#include "dbmanager.h"

#include <QMessageBox>

DbManager * DbManager::controller = nullptr;


DbManager::DbManager()
{
    db_manager = QSqlDatabase::addDatabase("QSQLITE");
    db_manager.setDatabaseName("/Users/JoseCardenas/european_cities.db");
//    db_manager.setDatabaseName("C:\\Users\\Jake\\Desktop\\european_cities.db");

    if (!db_manager.open())
    {
        qDebug() << "Database: ERROR unable to connect";
    }
    else
    {
        qDebug() << "Database: Connection running";
    }
}


DbManager * DbManager::getInstance()
{
    if(controller == nullptr)
    {
        controller = new DbManager();
    }
    return controller;
}


void DbManager::initDataBase()
{
    QSqlQuery query;


    /*
     * The Following three query executions create the tables inside out database
     */

    query.exec( "CREATE TABLE IF NOT EXISTS `Admin` ("
                " `Username`             TEXT,   "
                " `Password`             TEXT   );"
                );


    query.exec( "CREATE TABLE IF NOT EXISTS `Cities` ("
                " `City`          TEXT, "
                " `TotalPurchase` INT );"
                );


    query.exec( "CREATE TABLE IF NOT EXISTS `Distances` ("
                " `Starting`    TEXT,   "
                " `Ending`      TEXT,   "
                " `Distance`    INT   );"
                );

    query.exec( "CREATE TABLE IF NOT EXISTS `Items` ("
                " `City`             TEXT,   "
                " `Item`             TEXT,   "
                " `Cost`             DECIMAL(10,5),    "
                " `PurchaseCount`    INT,    "
                " `PurchaseAmtTotal` INT   );"
                );



    // checks if the query has executed
    if(query.exec())
        qDebug() << "\n query executed";
    else
        qDebug() << "\n query did not executed";


    // the file for reading in the distances is initialized
    //QFile distancesFile("/Users/JoseCardenas/Desktop/E/Distances-Table 1.csv");
    QFile distancesFile("C:\\Users\\Jake\\Desktop\\Distances-Table 1.csv");

    // the file for reading in the items is initialized
    //QFile itemsFile("/Users/JoseCardenas/Desktop/E/Foods-Table 1.csv");
    QFile itemsFile("C:\\Users\\Jake\\Desktop\\Foods-Table 1.csv");




    if (!distancesFile.open(QIODevice::ReadOnly))
        qDebug() << distancesFile.errorString();

    if (!itemsFile.open(QIODevice::ReadOnly))
        qDebug() << itemsFile.errorString();

    // the header of the distance file is read, so these values are not stored in the db
    distancesFile.readLine();

    // the header of the iteems file is read, so these values are not stored in the db
    itemsFile.readLine();

    // the next line read in stored inside a QByteArray variable
    QByteArray itemsLine = itemsFile.readLine();

    // the next line read in stored inside a QByteArray variable
    QByteArray distancesLine = distancesFile.readLine();





    QVector<QString> cities; // holds the names of the cities with in the distance file
    QString city;      // used for storing the name of the city that which the items and costs belong too
    QString tempItem;  // temporary item
    QString tempCost;  // temporary cost
    QVector<QString> costs;  // vector to hold the costs of a city
    QVector<QString> items;  // vector to hold the items of a city
    QStringRef subString;



    while(!itemsFile.atEnd())
    {

        // from the QByteArray variable, it splits it based on commas and only inserts the first element into the string (city)
        city = itemsLine.split(',').at(0);

        // reads the next that holds the items and the cost value
        itemsLine = itemsFile.readLine();



        // stores the items and cost, by spiiting itemsLine by commas and accessing the element inside the array
        tempItem = itemsLine.split(',').at(1);
        tempCost = itemsLine.split(',').at(2);

        // trims the prices, removes whitespace from the start and the end.
        tempCost = tempCost.trimmed();

        //        qDebug() << "tempCost: " << tempCost << endl;

        // creats a subsring that takes the $ from the string
        QStringRef subString(&tempCost, 1, tempCost.size()-1);

        // the subString is assigned back to tempCost
        tempCost = subString.toString();



        // checks if  the item is NULL or at the end of file
        while(tempItem != nullptr && !itemsFile.atEnd())
        {
            items.push_back(tempItem); // pushes back the item
            costs.push_back(tempCost); // pushes back the cost

            itemsLine = itemsFile.readLine(); // reads the next line

            tempItem = itemsLine.split(',').at(1); // stores the item into the tempVariable
            tempCost = itemsLine.split(',').at(2); // stores the item into the tempVariable

            // trims the prices, removes whitespace from the start and the end.
            tempCost = tempCost.trimmed();

            // creats a subsring that takes the $ from the string
            QStringRef subString(&tempCost, 1, tempCost.size()-1);

            // the subString is assigned back to tempCost
            tempCost = subString.toString();



        }
        // since the while loop exits out when the file is at the last line, we add the push back the last
        // elements that were on that line
        if(itemsFile.atEnd())
        {
            items.push_back(tempItem); // stores the final item
            costs.push_back(tempCost);  // stores the final item
        }

        // for loop - since the city is know the for loop is used to traverse throught the vector and
        // dump the values inside the database
        for(int index = 0; index < items.size(); index++)
        {
            query.prepare("INSERT INTO Items(City, Item, Cost) VALUES (:val1, :val2, :val3);");
            query.bindValue(":val1", city);
            query.bindValue(":val2", items[index]);
            query.bindValue(":val3", costs[index]);
            query.exec();
        }

        // clears the vectors for the next city
        items.clear();
        costs.clear();


    }

    QString startingCity; // the starting location
    QString endingCity;  //  the ending location
    QString distance;   // distance from one city to another
    bool check = false;

    while(!distancesFile.atEnd())
    {




        // from the QByteArray varibable, it is split based on commas and then assigned to each string based on the index within the array
        startingCity = distancesLine.split(',').at(0);
        endingCity   = distancesLine.split(',').at(1);
        distance     = distancesLine.split(',').at(2);

        // the line read from the csv file is read and stored inside a distancesLine
        distancesLine = distancesFile.readLine();

        query.prepare("INSERT INTO Distances(Starting, Ending, Distance) VALUES (:val1, :val2, :val3);");
        query.bindValue(":val1", startingCity);
        query.bindValue(":val2", endingCity);
        query.bindValue(":val3", distance);
        query.exec();


        /*
          * The following lines of code are for creating a list of the cities that are within the file.
          * Checking first if the vector to hold the cities is empty. Then running a foor loop & iterating through
          * the vector to check if the starting cities name is already inside the vector. If found, the boolean
          * variable 'check' is assigned to true. But if its not found then in the last if statement it will push back
          * the city into the cities vector. Finally the boolean variable 'check' is re-assigned to false for the next run.
          */

        // if the vector for cities is empty (meaning no city is in it) then push the city into the citises vector
        if(cities.empty())
            cities.push_back(startingCity);

        // const run time - checks the whole vector if an element(city) exists that with the same name
        for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++)
            if(*it == startingCity)
                check = true; // check becomes true if there is a city with same name

        // if an element is not found then it is pushed back into the vector
        if(!check)
            cities.push_back(startingCity);

        check = false;



    }


    // since the while loop exits out when the file is at the last line, theres still an element on the last line that doesnt get
    // inserted to the database
    if(distancesFile.atEnd())
    {
        // from the QByteArray varibable, it is split based on commas and then assigned to each string based on the index within the array
        startingCity = distancesLine.split(',').at(0);
        endingCity   = distancesLine.split(',').at(1);
        distance     = distancesLine.split(',').at(2);

        query.prepare("INSERT INTO Distances(Starting, Ending, Distance) VALUES (:val1, :val2, :val3);");
        query.bindValue(":val1", startingCity);
        query.bindValue(":val2", endingCity);
        query.bindValue(":val3", distance);
        query.exec();
    }


    // all elements of vector (cities) are inserted into the databse
    for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++)
    {
        query.prepare("INSERT INTO Cities(City) VALUES (:val1);");
        query.bindValue(":val1", *it);
        query.exec();
    }

}

QVector<QString> DbManager::getCities()
{
    QVector<QString> cities;
    QSqlQuery query;
    query.prepare("SELECT City FROM Cities");

    if(query.exec())
    {
        while(query.next())
        {
            QString newEuropeanCity;
            newEuropeanCity = query.value(0).toString();
            cities.push_back(newEuropeanCity);
        }
    }
    else
        qDebug() << "query didnt execute inside getCities()";

    return cities;


}

QVector<TraditionalFoodItems> DbManager::getMenuItems(const QString& city)
{
    QVector<TraditionalFoodItems> menuItems;
    QSqlQuery query;
    query.prepare("SELECT Item, Cost FROM Items WHERE City=(:val1)");
    query.bindValue(":val1", city);

    if(query.exec())
    {
        while(query.next())
        {
            TraditionalFoodItems item;
            item.name = query.value(0).toString();
            item.price = query.value(1).toDouble();
            menuItems.push_back(item);
        }
    }
    else
        qDebug() << "query didnt execute inside getCities()";


    return menuItems;
}


float DbManager::getDistanceInbetween(const QString & starting, const QString & ending)
{
    float distance = 0;
    QSqlQuery query;
    query.prepare("SELECT Distance FROM Distances WHERE Starting=(:val1) AND Ending=(:val2)");
    query.bindValue(":val1", starting);
    query.bindValue(":val2", ending);

    if(query.exec())
    {
        if(query.first())
            distance = query.value(0).toFloat();
    }
    else
        qDebug() << "query didnt execute:";


    return distance;

}

bool DbManager::itemExist(const QString & city, const QString & item)
{
    QSqlQuery query;
    query.prepare("SELECT City, Item FROM Items WHERE City=(:val1) AND Item=(:val2)");
    query.bindValue(":val1", city);
    query.bindValue(":val2", item);

    if(query.exec())
    {
        if(query.first())
        {
            return true;
        }
        else
            return false;
    }
    else
        return false;
}


void DbManager::deleteItem(const QString & city, const QString & item)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Items WHERE City=(:val1) AND Item=(:val2);");
    query.bindValue(":val1", city);
    query.bindValue(":val2", item);
    if(query.exec())
    {
        qDebug() << "item" << item << "deleted from " << city ;
    }
    else
        qDebug() << "item was not able to be deleted";
}

void DbManager::addItem(const QString & city, const QString & item, const double & price)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Items(City, Item, Cost) VALUES (:val1, :val2, :val3);");
    query.bindValue(":val1", city);
    query.bindValue(":val2", item);
    query.bindValue(":val3", price);
    if(query.exec())
    {

        qDebug() << "New items added: " << endl
                 << "newCity: " << city << endl
                 << "item: " << item << endl
                 << "price: " << price;
    }
    else
        qDebug() << "item was not able to be deleted";

}


void DbManager::addEuropeanCity(const QString & city)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Cities(City) VALUES (:val1);");
    query.bindValue(":val1", city);
    if(query.exec())
    {
        qDebug() << city << "has been added";
    }
    else
    {
        qDebug() << "could not add the city";
    }
}


void DbManager::readNewCitiesTxtFile()
{

    QSqlQuery query;

    // the file for reading in the items is initialized
    //QFile newdDistancesFile("/Users/JoseCardenas/Desktop/E/New Cities-Table 1.csv");
    QFile newdDistancesFile("C:\\Users\\Jake\\Desktop\\New Cities-Table 1.csv");

    if (!newdDistancesFile.open(QIODevice::ReadOnly))
        qDebug() << newdDistancesFile.errorString();


    // the header of the distance file is read, so these values are not stored in the db
    newdDistancesFile.readLine();

    // the next line read in stored inside a QByteArray variable
    QByteArray  distancesLine = newdDistancesFile.readLine();

    // qstring of cities is initialized with the current cities inside the database
    QVector<QString> cities = getCities();
    QString startingCity; // the starting location
    QString endingCity;  //  the ending location
    QString distance;   // distance from one city to another
    bool check = false;
    bool distanceExists = false;





    while(!newdDistancesFile.atEnd())
    {
        // from the QByteArray varibable, it is split based on commas and then assigned to each string based on the index within the array
        startingCity = distancesLine.split(',').at(0);
        endingCity   = distancesLine.split(',').at(1);
        distance     = distancesLine.split(',').at(2);

        query.prepare("SELECT Starting, Ending FROM Distances WHERE Starting=(:val1) AND Ending=(:val2) ");
        query.bindValue(":val1", startingCity);
        query.bindValue(":val2", endingCity);

        if(query.exec())
        {
            if(query.first())
                distanceExists =  true;
            else
                 distanceExists =  false;
        }
        else
            distanceExists = false;

        // the line read from the csv file is read and stored inside a distancesLine
        distancesLine = newdDistancesFile.readLine();

        // if - the distance doesnt exist in the database proceed forward
        if(!distanceExists)
        {

            query.prepare("INSERT INTO Distances(Starting, Ending, Distance) VALUES (:val1, :val2, :val3);");
            query.bindValue(":val1", startingCity);
            query.bindValue(":val2", endingCity);
            query.bindValue(":val3", distance);
            query.exec();

            /*
              * The following lines of code are for creating a list of the cities that are within the file.
              * Checking first if the vector to hold the cities is empty. Then running a foor loop & iterating through
              * the vector to check if the starting cities name is already inside the vector. If found, the boolean
              * variable 'check' is assigned to true. But if its not found then in the last if statement it will push back
              * the city into the cities vector. Finally the boolean variable 'check' is re-assigned to false for the next run.
              */

            // if the vector for cities is empty (meaning no city is in it) then push the city into the citises vector
            if(cities.empty())
                cities.push_back(startingCity);

            // const run time - checks the whole vector if an element(city) exists that with the same name
            for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++)
                if(*it == startingCity)
                     check = true; // check becomes true if there is a city with same name

            // if an element is not found then it is pushed back into the vector and the city is inserted into the database
            if(!check)
            {
                cities.push_back(startingCity);
                query.prepare("INSERT INTO Cities(City) VALUES (:val1);");
                query.bindValue(":val1", startingCity);
                query.exec();

            }

            check = false;
        }

    }

    // since the while loop exits out when the file is at the last line, theres still an element on the last line that doesnt get
    // inserted to the database
    if(newdDistancesFile.atEnd())
    {
        // from the QByteArray varibable, it is split based on commas and then assigned to each string based on the index within the array
        startingCity = distancesLine.split(',').at(0);
        endingCity   = distancesLine.split(',').at(1);
        distance     = distancesLine.split(',').at(2);

        query.prepare("SELECT Starting, Ending FROM Distances WHERE Starting=(:val1) AND Ending=(:val2) ");
        query.bindValue(":val1", startingCity);
        query.bindValue(":val2", endingCity);

        if(query.exec())
        {
            if(query.first())
                distanceExists =  true;
            else
                 distanceExists =  false;
        }
        else
            distanceExists = false;

        // if - the distance doesnt exist in the database proceed forward
        if(!distanceExists)
        {

            query.prepare("INSERT INTO Distances(Starting, Ending, Distance) VALUES (:val1, :val2, :val3);");
            query.bindValue(":val1", startingCity);
            query.bindValue(":val2", endingCity);
            query.bindValue(":val3", distance);
            query.exec();

            /*
              * The following lines of code are for creating a list of the cities that are within the file.
              * Checking first if the vector to hold the cities is empty. Then running a foor loop & iterating through
              * the vector to check if the starting cities name is already inside the vector. If found, the boolean
              * variable 'check' is assigned to true. But if its not found then in the last if statement it will push back
              * the city into the cities vector. Finally the boolean variable 'check' is re-assigned to false for the next run.
              */

            // if the vector for cities is empty (meaning no city is in it) then push the city into the citises vector
            if(cities.empty())
                cities.push_back(startingCity);

            // const run time - checks the whole vector if an element(city) exists that with the same name
            for(QVector<QString>::iterator it = cities.begin(); it != cities.end(); it++)
                if(*it == startingCity)
                     check = true; // check becomes true if there is a city with same name

            // if an element is not found then it is pushed back into the vector and the city is inserted into the database
            if(!check)
            {
                cities.push_back(startingCity);
                query.prepare("INSERT INTO Cities(City) VALUES (:val1);");
                query.bindValue(":val1", startingCity);
                query.exec();

            }

            check = false;
        }
    }
}
