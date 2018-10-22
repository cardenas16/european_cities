#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QFile>
#include <QTextCodec>
#include <algorithm>



struct TraditionalFoodItems
{
    QString name;
    double price;
};



class DbManager
{
public:
    DbManager();
    static DbManager * getInstance();
    void initDataBase();
    QVector<QString> getCities();
    QVector<TraditionalFoodItems> getMenuItems(const QString& city);
    float getDistanceInbetween(const QString & starting, const QString & ending);

    void readNewCitiesTxtFile();
    bool itemExist(const QString & city, const QString & item);

    void deleteItem(const QString & city, const QString & item);

    void addItem(const QString & city, const QString & item, const double & price);
    void addEuropeanCity(const QString & city);



private:
    QSqlDatabase db_manager;
    static DbManager * controller;


};

#endif // DBMANAGER_H
