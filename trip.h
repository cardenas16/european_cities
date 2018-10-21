#pragma once
#ifndef TRIP_H
#define TRIP_H

#include <QWidget>

namespace Ui {
class Trip;
}

class Trip : public QWidget
{
    Q_OBJECT

public:
    explicit Trip(QWidget *parent = nullptr);
    ~Trip();

private:
    Ui::Trip *ui;
};

#endif // TRIP_H
