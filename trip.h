#pragma once
#ifndef TRIP_H
#define TRIP_H

#include <QWidget>
#include <QMainWindow>
#include <QtWidgets>
#include <QSqlTableModel>
#include "city.h"
#include "dbmanager.h"
#include <QMap>

namespace Ui {
class Trip;
}

class Trip : public QMainWindow
{
    Q_OBJECT

public:
    explicit Trip(QWidget *parent = nullptr);
    explicit Trip(QWidget *parent = nullptr, QVector<City> = {});
    ~Trip();

private slots:
    void purchasedItem();

    void on_pushButton_clicked();

    void on_nextCityButton_clicked();

private:
    Ui::Trip *ui;
    QListWidget *citiesDisplayWidget;
    QVector<City> currentTrip;
    QVector<QMap<QString, int>> shoppingCart;
    QMap<QString, int> cityCart;
    int currentCityIndex;

    template <class Type>
    void deleteDynamicWidget(Type *widget);
    void populateShoppingTable(QString cityName);
};

#endif // TRIP_H
