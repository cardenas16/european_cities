#ifndef TRAVELERSMAINWINDOW_H
#define TRAVELERSMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <algorithm>
#include <QtWidgets>

#include <QDialog>


#include "admin.h"
#include "trip.h"
#include "test.h"
#include "city.h"
#include "dbmanager.h"
#include <QStringList>
#include <QLineEdit>
#include <QtWidgets>



namespace Ui {
class TravelersMainWindow;
}

class TravelersMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TravelersMainWindow(QWidget *parent = nullptr);
    ~TravelersMainWindow();

    void populateDisplay();


private slots:


    void selectedCity();

    void on_pb_NextCity_clicked();

    void on_pb_back_clicked();

   void openAdminWindow();
   void openTripOperationsWindow();

   void on_pushButton_3_clicked();

   void on_makeCustomTripButton_clicked();

   void on_pushErrorButton_clicked();

   void on_makeCustomTripButtonConfirm_clicked();

   void on_makeCustomSubTripButtonConfirm_clicked();

   void on_subsequentTripErrorButton_clicked();

private:
    Ui::TravelersMainWindow *ui;
    Admin * adminWindow;
    Trip * tripOperations;
    test * testWindow;
    QDialog * myDialog; 
    QString startingLocation;

    bool vectorContains(const QVector<QString>&, const QString);
    QVector<QString> getSelectedCities(QListWidget *widget, bool initialCityOnly = false);
    QVector<City> modifiedNextClosest(QVector<City> cities, QVector<QString> selectedCities, QString startingCity = "");
    bool contains(const QVector<City> &cities, const QString location);
    void populateSubsequentCustomTripOptions();
};

#endif // TRAVELERSMAINWINDOW_H
