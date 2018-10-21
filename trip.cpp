#include "trip.h"
#include "ui_trip.h"

Trip::Trip(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Trip)
{
    ui->setupUi(this);
    citiesDisplayWidget = nullptr;
    currentCityIndex = 0;
}

Trip::Trip(QWidget *parent, QVector<City> trip) :
    QMainWindow(parent),
    ui(new Ui::Trip)
{
    const QStringList HEADERS = {"Item", "Cost"};
    ui->setupUi(this);
    citiesDisplayWidget = new QListWidget;
    ui->purchaseHistory->setColumnCount(2);
    ui->purchaseHistory->setHorizontalHeaderLabels(HEADERS);
    currentTrip = trip;
    cityCart.clear();
    currentCityIndex = 0;

    for (int i = 0; i < currentTrip.size(); i++)
    {
        QListWidgetItem *listItem = new QListWidgetItem(currentTrip[i].getName());
        citiesDisplayWidget->addItem(listItem);
    }

    ui->tripOverviewLayout->addWidget(citiesDisplayWidget);

    QVector<TraditionalFoodItems> menuItems = DbManager::getInstance()->getMenuItems(currentTrip[currentCityIndex].getName());
    ui->shoppingButtonsLabel->setText("Displaying Purchase Options From: " + currentTrip[currentCityIndex].getName());
    citiesDisplayWidget->item(currentCityIndex)->setBackgroundColor(QColor(255, 255, 102));

    for (int i = 0; i < menuItems.size(); i++)
    {
        // create a button with the name and price
        QPushButton *foodItem = new QPushButton(menuItems[i].name + "\nPrice: $" + QString::number(menuItems[i].price));
        // name the object
        foodItem->setObjectName(menuItems[i].name);
        // make it do something when clicked
        connect(foodItem, SIGNAL(clicked()), this, SLOT(purchasedItem()));
        foodItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        ui->shoppingButtonsLayout->addWidget(foodItem);
        cityCart.insert(menuItems[i].name, 0);
    }
}

Trip::~Trip()
{
    delete ui;
    deleteDynamicWidget(citiesDisplayWidget);
}

template <class Type>
void Trip::deleteDynamicWidget(Type *widget)
{
    if (widget != nullptr)
    {
        delete widget;
        widget = nullptr;
    }
}

void Trip::purchasedItem()
{
    QString name = qobject_cast<QPushButton*>(sender())->objectName();
    cityCart[name]++;
    QVector<TraditionalFoodItems> menuItems = DbManager::getInstance()->getMenuItems(currentTrip[currentCityIndex].getName());
    double price = 0.00;

    for (int i = 0; i < menuItems.size(); i++)
    {
        if (menuItems[i].name == name)
        {
            price = menuItems[i].price;
            break;
        }
    }

    int rowCount = ui->purchaseHistory->rowCount();
    ui->purchaseHistory->insertRow(rowCount);
    ui->purchaseHistory->setItem(rowCount, 0, new QTableWidgetItem(name));
    //ui->purchaseHistory->setItem(rowCount, 1, new QTableWidgetItem("$" + QString::number(price)));

}

void Trip::on_pushButton_clicked()
{
    for (QMap<QString, int>::Iterator it = cityCart.begin(); it != cityCart.end(); it++)
        qDebug() << it.key() << ": " << it.value();
}

void Trip::on_nextCityButton_clicked()
{
    while (ui->shoppingButtonsLayout->count())
    {
        QWidget* widget = ui->shoppingButtonsLayout->itemAt(0)->widget();
        if(widget)
        {
            ui->shoppingButtonsLayout->removeWidget(widget);
            delete widget;
        }
    }

    shoppingCart.push_back(cityCart);
    cityCart.clear();
    citiesDisplayWidget->item(currentCityIndex)->setBackgroundColor(QColor(255, 255, 255));
    currentCityIndex++;

    if (currentCityIndex > currentTrip.size())
        return; // replace with generation of shopping cart (final)

    citiesDisplayWidget->item(currentCityIndex)->setBackgroundColor(QColor(255, 255, 102));
    QVector<TraditionalFoodItems> menuItems = DbManager::getInstance()->getMenuItems(currentTrip[currentCityIndex].getName());
    ui->shoppingButtonsLabel->setText("Displaying Purchase Options From: " + currentTrip[currentCityIndex].getName());

    for (int i = 0; i < menuItems.size(); i++)
    {
        // create a button with the name and price
        QPushButton *foodItem = new QPushButton(menuItems[i].name + "\nPrice: $" + QString::number(menuItems[i].price));
        // name the object
        foodItem->setObjectName(menuItems[i].name);
        // make it do something when clicked
        connect(foodItem, SIGNAL(clicked()), this, SLOT(purchasedItem()));
        foodItem->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        ui->shoppingButtonsLayout->addWidget(foodItem);
        cityCart.insert(menuItems[i].name, 0);
    }
}







