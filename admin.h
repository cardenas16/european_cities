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
#include "spinbox.h"
#include "customdelegate.h"





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


    void on_addItemButton_clicked();


    void saveChanges();



    void on_deleteButton_clicked();

    void on_newCityButton_clicked();


    void on_backButton_newCity_clicked();



    void on_saveButton_newCity_clicked();

private:
    Ui::Admin *ui;
    QSqlTableModel * tableModel;

    QStringListModel * citiesModel;

    SpinBox * newPriceDelegate;

    customDelegate * newItemDelegate;

    QModelIndex highlightedCell;


    bool newRow;

    QVector<QString> cities;


     QStringList citiesList;
    int comboBoxIndex;




};

#endif // ADMIN_H

