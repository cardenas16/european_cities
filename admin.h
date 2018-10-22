#ifndef ADMIN_H
#define ADMIN_H


#include <QMainWindow>
#include <QSqlTableModel>
#include <QStringListModel>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QSqlRecord>
#include <QMessageBox>


#include "dbmanager.h"
#include "spinboxdelegate.h"
#include "newitemdelegate.h"





namespace Ui {
class Admin;
}

class Admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

    void initialTableDisplay();


private slots:
    // once clicked on, it validates the usersname and password
    void on_loginButton_clicked();

    void setTableView(int index);

    void cellClicked(const QModelIndex &index);

    void newItemValidation(QModelIndex index);



    void saveChanges();

    void on_button_addItem_clicked();

    void on_button_deleteItem_clicked();




    void on_newCityButton_clicked();


    void on_backButton_newCity_clicked();



    void on_saveButton_newCity_clicked();


    void on_button_importCities_clicked();


private:
    Ui::Admin *ui;
    QSqlTableModel * tableModel;

    QStringListModel * citiesModel;

    SpinBoxDelegate * newPriceDelegate;

    NewItemDelegate * newItemDelegate;

    QModelIndex highlightedCell;


    bool newRow;


    QVector<QString> cities;


     QStringList citiesList;
    int comboBoxIndex;




};

#endif // ADMIN_H
