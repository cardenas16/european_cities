#include "test.h"
#include "ui_test.h"

test::test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this);

    model->setTable("Items");
    model->select();

    ui->tableView->setModel(model);



}

test::~test()
{
    delete ui;
}
