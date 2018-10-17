#include "city.h"

City::City()
{
    cityName = "NULL";
    distanceFromPreviousCity = 0;
}

City::City(QString x, int y)
{
    cityName = x;
    distanceFromPreviousCity = y;
}

QString City::getName() const
{
    return cityName;
}

int City::getDistance() const
{
    return distanceFromPreviousCity;
}

void City::setName(QString x)
{
    cityName = x;
}

void City::setDistance(int x)
{
    distanceFromPreviousCity = x;
}

QVector<City> nextClosest(QVector<City> cities, int numCities, QString startingCity)
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
        if (!contains(cities, query.value(0).toString()))
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
    if (cities.size() >= numCities)
        return cities;

    // Else, continue calling the algorithm
    else
        return nextClosest(cities, numCities);
}

// Helper function for our Algorithm
// Checks to see whether or not we've visited a city before
bool contains(const QVector<City> &cities, const QString location)
{
    for (int i = 0; i < cities.size(); i++)
    {
        if (cities[i].getName() == location)
            return true;
    }

    return false;
}

// Function to calculate the total distance traveled during the trip
// Takes a QVector of Cities and adds the distance between each city in
// in order to get the total distance traveled
int getDistanceTraveled(const QVector<City> &cities)  // Story 8
{
    int distanceTraveled = 0;

    for (int i = 0; i < cities.size(); i++)
        distanceTraveled += cities[i].getDistance();

    return distanceTraveled;
}

// Function to retrieve surrounding cities and their distances from
// a given base city
QVector<City> getSurroundingCities(const QString baseCity)
{
    QSqlQuery query;	// The variable we're accessing the database with
    QVector<City> nearbyCities;
    City tempCity = City();

    // Request data from the database based on the base city
    query.prepare("SELECT Ending, Distance from Distances WHERE Starting=(:val1)");
    query.bindValue(":val1", baseCity);
    query.exec();

    // While there are still elements in the query
    while (query.next())
    {
        if (!contains(nearbyCities, query.value(0).toString()))
        {
            tempCity.setName(query.value(0).toString());
            tempCity.setDistance(query.value(1).toInt());
            nearbyCities.push_back(tempCity);
        }
    }

    return nearbyCities;
}

void selectionSort(QVector<City> &cities)
{
    City temp;
    int smallestIndex;

    for (int index = 0; index < cities.size() - 1; index++)
    {
        smallestIndex = index;

        for (int location = index + 1; location < cities.size(); location++)
        {
            if (cities[location].getDistance() < cities[smallestIndex].getDistance())
            {
                smallestIndex = location;
            }
        }

        temp = cities[smallestIndex];
        cities[smallestIndex] = cities[index];
        cities[index] = temp;
    }
}
