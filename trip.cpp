#include "trip.h"
#include "ui_trip.h"

Trip::Trip(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Trip)
{
    ui->setupUi(this);
    citiesDisplayWidget = nullptr;
    finalShoppingCart = nullptr;
    currentCityIndex = 0;
    backgroundImageDisplay(currentTrip, currentCityIndex);



}

Trip::Trip(QWidget *parent, QVector<City> trip) :
    QMainWindow(parent),
    ui(new Ui::Trip)
{
    ui->setupUi(this);
    citiesDisplayWidget = new QListWidget;
    ui->stackedWidget->setCurrentIndex(Shopping);
    ui->purchaseHistory->setColumnCount(2);
    ui->purchaseHistory->setHorizontalHeaderLabels(HEADERS);
    ui->purchaseHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->purchaseHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->purchaseHistory->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    currentTrip = trip;
    connect(ui->returnToMainMenuButton, SIGNAL(clicked()), this->parent(), SLOT(resetTripState()));

    cityCart.clear();
    currentCityIndex = 0;



    for (int i = 0; i < currentTrip.size(); i++)
    {
        QListWidgetItem *listItem = new QListWidgetItem(currentTrip[i].getName());
        citiesDisplayWidget->addItem(listItem);
    }

    ui->tripOverviewLayout->addWidget(citiesDisplayWidget);

    QVector<TraditionalFoodItems> menuItems = DbManager::getInstance()->getMenuItems(currentTrip[currentCityIndex].getName());
    ui->shoppingButtonsLabel->setText("Displaying Purchase Options \nFrom: " + currentTrip[currentCityIndex].getName());
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
     //backgroundImageDisplay(currentTrip, currentCityIndex);

}

Trip::~Trip()
{
    delete ui;
    deleteDynamicWidget(citiesDisplayWidget);
    deleteDynamicWidget(finalShoppingCart);
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


void Trip::backgroundImageDisplay( QVector<City> currentTrip, int currentCityIndex)
{

    qDebug() << "currentTrip: " << currentTrip[currentCityIndex].getName();
    QString cityName = currentTrip[currentCityIndex].getName();
    QVector<QString> cities =  DbManager::getInstance()->getCities();
    int indexOfCity;
    for( int index = 0; index < cities.size(); index++)
    {
        if(cities.at(index) == cityName)
            indexOfCity = index;

    }

    QString path;


qDebug() << "index:" << indexOfCity;
    switch (indexOfCity) {
    case 0:
        path = ":/resources/img/amsterdam.jpg";
        break;
    case 1:
        path = ":/resources/img/berlin.jpg";
        break;
    case 2:
        path = ":/resources/img/brussels2.jpg";
        break;
    case 3:
        path = ":/resources/img/budapest2.jpeg";
        break;
    case 4:
        path = ":/resources/img/hamburg2.jpg";
        break;
    case 5:
        path = ":/resources/img/lisbon.jpg";
        break;
    case 6:
        path = ":/resources/img/london2.jpg";
        break;
    case 7:
        path = ":/resources/img/madrid.jpg";
        break;
    case 8:
        path = ":/resources/img/paris.jpg";
        break;
    case 9:
        path = ":/resources/img/prague.jpg";
        break;
    case 10:
        path = ":/resources/img/rome.jpg";
        break;
    case 11:
        path = ":/resources/img/stockholm2.jpg";
        break;
    case 12:
        path = ":/resources/img/vienna.jpg";
        break;
    default:
        path = ":/resources/img/main_window_backgroudn.jpg";



    }


    QPixmap bkgnd(path);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
//     this->repaint();


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

    ui->purchaseHistory->insertRow(0);
    ui->purchaseHistory->setItem(0, 0, new QTableWidgetItem(name));
    ui->purchaseHistory->setItem(0, 1, new QTableWidgetItem("$" + QString::number(price)));
    ui->purchaseHistory->item(0, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->purchaseHistory->item(0, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
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
    totalSpentPerCity.push_back(getTotalSpentPerCity(cityCart));
    citiesDisplayWidget->item(currentCityIndex)->setBackgroundColor(QColor(255, 255, 255));
    cityCart.clear();

    currentCityIndex++;



    if (currentCityIndex >= currentTrip.size())
    {
        populateFinalTripMetrics();
        return;
    }

    citiesDisplayWidget->item(currentCityIndex)->setBackgroundColor(QColor(255, 255, 102));
    QVector<TraditionalFoodItems> menuItems = DbManager::getInstance()->getMenuItems(currentTrip[currentCityIndex].getName());
    ui->shoppingButtonsLabel->setText("Displaying Purchase Options From: \n" + currentTrip[currentCityIndex].getName());


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
    backgroundImageDisplay(currentTrip, currentCityIndex);
}

void Trip::on_clearHistoryButton_clicked()
{
    while (ui->purchaseHistory->rowCount())
        ui->purchaseHistory->removeRow(0);

    ui->purchaseHistory->setHorizontalHeaderLabels(HEADERS);
}

bool Trip::checkForEmptyNodes(QList<QTreeWidgetItem*> items)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i]->childCount() == 0)
            return true;
    }

    return false;
}

void Trip::removeEmptyNodes(QList<QTreeWidgetItem*> &items)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i]->childCount() == 0)
        {
            items.removeAt(i);
            return;
        }
    }
}

double Trip::getTotalSpentPerCity(QMap<QString, int> &cityCart)
{
    double totalSpent = 0.00;
    double price = 0.00;
    QMap<QString, int>::Iterator it;
    QVector<TraditionalFoodItems> menuItems = DbManager::getInstance()->getMenuItems(currentTrip[currentCityIndex].getName());

    for (QMap<QString, int>::Iterator it = cityCart.begin(); it != cityCart.end(); it++)
    {
        for (int i = 0; i < menuItems.size(); i++)
        {
            if (menuItems[i].name == it.key())
            {
                price = menuItems[i].price;
                totalSpent += (price * it.value());
                break;
            }
        }
    }

    return totalSpent;
}

void Trip::populateFinalTripMetrics()
{
    ui->stackedWidget->setCurrentIndex(FinalTrip);
    QMap<QString, int>::Iterator it;
    finalShoppingCart = new QTreeWidget();
    QList<QTreeWidgetItem*> items;
    QTreeWidgetItem *child;
    double grandTotal = 0.00;
    int totalItems = 0;

    finalShoppingCart->setColumnCount(1);

    for (int i = 0; i < currentTrip.size(); i++)
    {
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(currentTrip[i].getName()) + " ($" + QString::number(totalSpentPerCity[i]) + ")")));

        for (it = shoppingCart[i].begin(); it != shoppingCart[i].end(); it++)
        {
            if (it.value() > 0)
            {
                child = new QTreeWidgetItem(items[i], QStringList(QString(it.key() + " (" + QString::number(it.value()) + ")")));
                items[i]->addChild(child);
                totalItems += it.value();
            }
        }
    }

    while (checkForEmptyNodes(items))
    {
        removeEmptyNodes(items);
    }

    for (int i = 0; i < totalSpentPerCity.size(); i++)
        grandTotal += totalSpentPerCity[i];

    finalShoppingCart->setHeaderLabel("Cities ($" + QString::number(grandTotal) + ")");

    finalShoppingCart->addTopLevelItems(items);
    ui->treeWidgetLayout->addWidget(finalShoppingCart);

    ui->tripRecap->addItem("During your trip, you visited a total of " + QString::number(currentTrip.size()) + " cities!");
    ui->tripRecap->addItem("During your trip, you traveled a total distance of " + QString::number(getDistanceTraveled(currentTrip)) + " kilometers!");
    ui->tripRecap->addItem("During your trip, you purchased a total of " + QString::number(totalItems) + " items!");
    ui->tripRecap->addItem("During your trip, you spent a total of $" + QString::number(grandTotal) + "!");
    ui->tripRecap->addItem("\n");
    ui->tripRecap->addItem("Thank you for choosing Squad Planning Services!");
    ui->tripRecap->addItem("\"When you're with us, you're always part of the Squad!\"");
}

void Trip::on_returnToMainMenuButton_clicked()
{
    qobject_cast<QMainWindow*>(parent())->show();
    this->~Trip();
}
