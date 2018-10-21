#ifndef TRAVELERSMAINWINDOW_H
#define TRAVELERSMAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <algorithm>

#include <QDialog>


#include "admin.h"






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

   void on_pushButton_3_clicked();

private:
    Ui::TravelersMainWindow *ui;
    Admin * adminWindow;

    QDialog * myDialog;
};

#endif // TRAVELERSMAINWINDOW_H
