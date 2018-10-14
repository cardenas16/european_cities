#ifndef TEST_H
#define TEST_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class test;
}

class test : public QMainWindow
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = nullptr);
    ~test();

private:
    Ui::test *ui;
    QSqlTableModel * model;

};

#endif // TEST_H
