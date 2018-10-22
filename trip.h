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

const QStringList HEADERS = {"Item", "Cost"};
enum tripStackedPages { Shopping = 0, FinalTrip };

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
    void on_nextCityButton_clicked();
    void on_clearHistoryButton_clicked();
    void on_returnToMainMenuButton_clicked();

private:
    Ui::Trip *ui;
    QListWidget *citiesDisplayWidget;
    QVector<City> currentTrip;
    QVector<QMap<QString, int>> shoppingCart;
    QMap<QString, int> cityCart;
    QTreeWidget *finalShoppingCart;
    QVector<double> totalSpentPerCity;
    int currentCityIndex;

    template <class Type>
    void deleteDynamicWidget(Type *widget);
    void populateShoppingTable(QString cityName);
    bool checkForEmptyNodes(QList<QTreeWidgetItem*> items);
    void removeEmptyNodes(QList<QTreeWidgetItem*> &items);
    double getTotalSpentPerCity(QMap<QString, int> &cityCart);
    void populateFinalTripMetrics();

};

#endif // TRIP_H
