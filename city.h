#ifndef CITY_H
#define CITY_H

#include <QString>
#include <QVector>
#include "dbmanager.h"

class City
{
public:
    City();
    City(QString, int);
    int getDistance() const;
    QString getName() const;
    void setDistance(int);
    void setName(QString);

private:
    QString cityName;
    int distanceFromPreviousCity;
};

struct TraditionalFoodItems
{
    QString name;
    double price;
};

QVector<City> nextClosest(QVector<City> cities, int numCities, QString startingCity = "");
bool contains(const QVector<City> &cities, const QString location);
int getDistanceTraveled(const QVector<City> &cities);  // Story 8
QVector<City> getSurroundingCities(const QString baseCity);
void selectionSort(QVector<City> &cities);

#endif // CITY_H
