#ifndef ADMIN_H
#define ADMIN_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QStringListModel>
#include <QSqlQuery>
#include <QModelIndex>


#include "dbmanager.h"


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



    void on_addItemButton_clicked();

    void test(QModelIndex index);

    void onTableClicked(const QSqlRecord &index);

    void selectedRow(QModelIndex index);





    void on_deleteButton_clicked();

private:
    Ui::Admin *ui;
    QSqlTableModel * tableModel;
    QStringListModel * citiesModel;
    bool repeatedItem;
    int inputRuns;
    QModelIndex highlightedCell;


};

#endif // ADMIN_H


