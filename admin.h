#ifndef ADMIN_H
#define ADMIN_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QStringListModel>
#include <QSqlQuery>


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



private:
    Ui::Admin *ui;
    QSqlTableModel * tableModel;
    QStringListModel * citiesModel;



};

#endif // ADMIN_H


/*
 *



private slots:
    void on_loginButton_clicked();

    void setTableView(int index);

    void on_addItemButton_clicked();

    void on_saveButton_clicked();

 */
