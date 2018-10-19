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


private slots:
    void on_loginButton_clicked();

    void setTableView(int index);

    void onTableClicked(const QModelIndex &index);

    void test(QModelIndex index);


    void on_addItemButton_clicked();

    void resetNewRow();

private:
    Ui::Admin *ui;
    QSqlTableModel * tableModel;
    QStringListModel * citiesModel;

    SpinBox * mySpinBox;

    customDelegate * myDelegate;


    bool newRow;





};

#endif // ADMIN_H

