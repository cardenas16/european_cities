#pragma once
#ifndef DBMANAGER_H
#define DBMANAGER_H


#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QFile>
#include <QTextCodec>
#include <algorithm>
#include "city.h"

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

    void readInTxtFile();
    bool itemExist(const QString & city, const QString & item);
    void addItem(const QString& city, const QString& item, const double & price);
    void deleteItem(const QString& city, const QString& item);

private:
    QSqlDatabase db_manager;
    static DbManager * controller;


};

#endif // DBMANAGER_H
