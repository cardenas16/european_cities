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
#include "city.h"
#include <QStringList>
#include <QLineEdit>
#include <QtWidgets>

// 'stackedPages' and 'buttonPages' represent the stackedWidget pages
// By creating an enum for them, indexes are easier to follow and use in function definitions
enum stackedPages { DisplayCities = 0, DisplayFoodItems, CheckableCities, ErrorPage, GeneratedTrip, LondonTrip };
enum buttonPages { On = 0, Off };
enum currentStep { InitialCity = 0, AdditionalCities, London };

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
    void methodOneTooPopluate(QVector<QString> cities);
     void methodTwoTooPopluate(QVector<QString> cities);
    void methodThreeTooPopluate(QVector<QString> cities);

private slots:
   void selectedCity();
   void on_pb_NextCity_clicked();
   void on_pb_back_clicked();
   void openAdminWindow();
   void openTripOperationsWindow();
   void on_makeCustomTripButton_clicked();
   void on_confirmChoicesButton_clicked();
   void on_errorButton_clicked();
   void on_takeLondonTripButton_clicked();
   void on_visitInitialCities_clicked();
   void on_confirmGeneratedTripButton_clicked();
   void adminLoggedOut();
   void on_obtainCitiesLineEdit_returnPressed();
   void on_obtainCitiesLineEdit_editingFinished();

private:
    Ui::TravelersMainWindow *ui;
    Admin * adminWindow;
    Trip * tripOperations;
    QDialog * myDialog;
    QString startingLocation;
    QListWidget *citySelectionWidget;
    int currentStep;
    QVector<City> currentTrip;
    int upperBound;

    bool vectorContains(const QVector<QString>&, const QString);
    QVector<QString> getSelectedCities(bool initialCityOnly = false);
    QVector<City> modifiedNextClosest(QVector<City> cities, QVector<QString> selectedCities, QString startingCity = "");
    bool contains(const QVector<City> &cities, const QString location);
    void populateSubsequentCustomTripOptions();
    void deleteCitySelectionWidget();
    int validateNumberOfCities();
    void processLondonTrip();
    void generateLondonTrip(int numCities);
};

#endif // TRAVELERSMAINWINDOW_H
