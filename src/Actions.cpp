

#include <vector>
#include <string>
#include "../include/Action.h"
#include "../include/WareHouse.h"
#include "../include/Order.h"
#include "../include/Volunteer.h"
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "../include/Action.h"
extern WareHouse *backup;

BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) {}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string Msg)
{
    status = ActionStatus::ERROR;
    errorMsg = Msg;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

//////////////AddOrder///////////////
AddOrder::AddOrder(int id) : BaseAction(), customerId(id) {}

void AddOrder::act(WareHouse &wareHouse)
{

    if (wareHouse.checkIfCustomerExsists(customerId))
    {
        int isAdded = wareHouse.getCustomer(customerId).addOrder(wareHouse.getOrderCounter()); // Trys to add an order (expected = orderID, if customer can't order will return -1)
        if (isAdded == -1)
        {
            error("Customer can't order!");
            std::cout << "ERROR: " << getErrorMsg() << endl;
        }
        else
        { // Customer could order
            int orderDistance = wareHouse.getCustomer(customerId).getCustomerDistance();
            Order *newOrder = new Order(isAdded, customerId, orderDistance);
            wareHouse.addOrder(newOrder);
            complete();
        }
    }
    else
    {
        error("Customer doesn't exist!");
        std::cout << "ERROR: " << getErrorMsg() << endl;
    }
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}
string AddOrder::toString() const
{
    if (ActionStatus::COMPLETED == getStatus())
    {
        return "order " + to_string(customerId) + " COMPLETED";
    }
    return "order " + to_string(customerId) + " ERROR";
}
//////////////////////////////////////////AddCustomer///////////////////////////////////
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) : BaseAction(), customerName(customerName), customerType(CustomerType(convertCustomerType(customerType))), distance(distance), maxOrders(maxOrders)
{
}
void AddCustomer::act(WareHouse &wareHouse)
{
    if (customerType == CustomerType::Soldier)
    {
        Customer *newCustomer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(newCustomer);
    }

    if (customerType == CustomerType::Civilian)
    {
        Customer *newCustomer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(newCustomer);
    }

    complete();
}
int AddCustomer::convertCustomerType(string customerType)
{
    int ret = -1;
    if (customerType == "soldier")
    {
        ret = 0;
    }
    if (customerType == "civilian")
    {
        ret = 1;
    }
    return ret;
}

AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}
string AddCustomer::toString() const
{
    string res = "customer " + customerName;
    if (customerType == CustomerType::Soldier)
    {
        res += " soldier ";
    }
    else if (customerType == CustomerType::Civilian)
    {
        res += " civilian ";
    }
    res += to_string(distance) + " " + to_string(maxOrders) + " ";

    if (ActionStatus::COMPLETED == getStatus())
    {
        return res + "COMPLETED";
    }
    return res + "ERROR";
}
///////////////////////////////PrintOrderStatus////////////////////////////////////
PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id) {}

void PrintOrderStatus::act(WareHouse &warehouse)
{
    if (!warehouse.checkIfOrderExsists(orderId))
    {
        error("Order doesn't exist.");
        cout << "Error: " << getErrorMsg() << endl;
        return;
    }
    Order order = warehouse.getOrder(orderId);
    cout << "OrderId: " + to_string(orderId) << endl;
    cout << "OrderStatus: " + to_String(order.getStatus()) << endl;
    cout << "CustomerID: " + to_string(order.getCustomerId()) << endl;
    cout << "Collector: " + volunteerIdToString(order.getCollectorId()) << endl;
    cout << "Driver: " + volunteerIdToString(order.getDriverId()) << endl;
    complete();
}

PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
    if (getStatus() == ActionStatus::COMPLETED)
        return "orderStatus " + to_string(orderId) + " COMPLETED";

    return "orderStatus " + to_string(orderId) + " ERROR";
}

std::string BaseAction::to_String(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::PENDING:
        return "Pending";
    case OrderStatus::COLLECTING:
        return "Collecting";
    case OrderStatus::DELIVERING:
        return "Delivering";
    case OrderStatus::COMPLETED:
        return "Completed";
    default:
        return "Unknown"; // Handle unknown status, if necessary
    }
}
std::string BaseAction::volunteerIdToString(int volId)
{
    if (volId == -1)
        return "None";
    return to_string(volId);
}
////////////////////////////// printCustomerStatus////////////////////////////

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (!wareHouse.checkIfCustomerExsists(customerId))
    {
        error("Customer doesn't exsist.");
        cout << "ERROR: " + getErrorMsg() << endl;
        return;
    }
    cout << "CustomerID: " + to_string(customerId) << endl;
    for (int orderId : wareHouse.getCustomer(customerId).getOrdersIds())
    {
        cout << "OrderId: " << orderId << " OrderStatus: " << to_String(wareHouse.getOrder(orderId).getStatus()) << endl;
    }
    cout << "numOrdersLeft: " + to_string(wareHouse.getCustomer(customerId).getNumOrdersLeft()) << endl;
    complete();
}

PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
    {
        return "customerStatus " + to_string(customerId) + " ERROR";
    }
    return "customerStatus " + to_string(customerId) + " COMPLETED";
}
////////////////////////////////////////PrintVolunteerStatus/////////////////////////////////
PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    if (!wareHouse.checkIfVolunteerExsists(volunteerId))
    {
        error("Volunteer doesn't exist.");
        cout << "ERROR: " + getErrorMsg() << endl;
        return;
    }

    cout << "VolunteerID: " + to_string(volunteerId) << endl;

    auto &volunteer = wareHouse.getVolunteer(volunteerId);

    // Declare myVolunteer outside the if-else blocks
    Volunteer *myVolunteer = nullptr;

    if (volunteer.type() == "Driver")
    {
        myVolunteer = dynamic_cast<DriverVolunteer *>(&volunteer);
    }
    else if (volunteer.type() == "Collector")
    {
        myVolunteer = dynamic_cast<CollectorVolunteer *>(&volunteer);
    }
    else if (volunteer.type() == "LimitedDriver")
    {
        myVolunteer = dynamic_cast<LimitedDriverVolunteer *>(&volunteer);
    }
    else // LimitedCollector
    {
        myVolunteer = dynamic_cast<LimitedCollectorVolunteer *>(&volunteer);
    }

    if (!myVolunteer)
    {
        // Handle the case where dynamic_cast failed
        error("Failed to cast to the correct volunteer type.");
        cout << "ERROR: " + getErrorMsg() << endl;
        return;
    }

    // Continue with the rest of your code using myVolunteer
    string isVolBusy;
    if (myVolunteer->isBusy())
        isVolBusy = "True";
    else
        isVolBusy = "False";
    cout << "IsBusy: " << isVolBusy << endl;
    cout << "OrderID: " << volunteerIdToString(myVolunteer->getActiveOrderId()) << endl; // volunteerIdToString also works for order IDs, might change later
    cout << "TimeLeft: ";
    if (!myVolunteer->isBusy())
    {
        cout << "None" << endl; // If TimeLeft/DistanceLeft == 0 than isBusy is false.
    }
    else if (myVolunteer->type() == "LimitedCollector" || myVolunteer->type() == "Collector")
    {
        CollectorVolunteer *collectorVolunteerPtr = dynamic_cast<CollectorVolunteer *>(myVolunteer);
        cout << to_string(collectorVolunteerPtr->getTimeLeft()) << endl;
    }
    else if (myVolunteer->type() == "LimitedDriver" || myVolunteer->type() == "Driver")
    {
        DriverVolunteer *driverVolunteerPtr = dynamic_cast<DriverVolunteer *>(myVolunteer);
        cout << to_string(driverVolunteerPtr->getDistanceLeft()) << endl;
    }
    cout << "OrdersLeft: ";
    if (myVolunteer->type() == "LimitedDriver")
    {
        LimitedDriverVolunteer *driverVolunteerPtr = dynamic_cast<LimitedDriverVolunteer *>(myVolunteer);
        cout << to_string(driverVolunteerPtr->getMaxOrders()) << endl;
    }
    else if (myVolunteer->type() == "LimitedCollector")
    {
        LimitedCollectorVolunteer *collectorVolunteerPtr = dynamic_cast<LimitedCollectorVolunteer *>(myVolunteer);
        cout << to_string(collectorVolunteerPtr->getMaxOrders()) << endl;
    }
    else
        cout << "No Limit" << endl;
    complete();
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    if (getStatus() == ActionStatus::COMPLETED)
        return "volunteerStatus " + to_string(volunteerId) + " COMPLETED";

    return "volunteerStatus " + to_string(volunteerId) + " ERROR";
}

////////////////////////SimulateStep/////////////////////////

SimulateStep::SimulateStep(int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}
void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {

        // 1. iterate pending orders vector and assign orders to free volunteers
        assignJobs(wareHouse);

        // 2. iterate busy volunteers: decrease distance of drivers and decrease collectors cooldown
        promoteOrders(wareHouse);

        // 3. iterate volunteers and check who reached their destinations / finnished collecting and changes the order status and move to pending/completed vector.
        freeUpVolunteers(wareHouse);

        // 4. iterate volunteers and remove limited ones from the vector
        fireVolunteers(wareHouse);

        // 5. enjoy!
    }
    complete();
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

string SimulateStep::toString() const
{
    return "step " + to_string(numOfSteps) + " COMPLETED";
}

void SimulateStep::assignJobs(WareHouse &wareHouse)
{
    vector<Order *>::const_iterator it = wareHouse.getPendingOrdersVector().begin();
    while (it != wareHouse.getPendingOrdersVector().end())
    {
        Order *order = *it;
        bool found = false;
        if (order->getStatus() == OrderStatus::PENDING)
        {

            auto volunteerIt = wareHouse.getVolunteerVector().begin();
            while (!found && volunteerIt != wareHouse.getVolunteerVector().end())
            {
                auto volunteer = *volunteerIt;

                if (volunteer->type() == "Collector" || volunteer->type() == "LimitedCollector")
                {
                    if (volunteer->canTakeOrder(*order))
                    {

                        volunteer->acceptOrder(*order);
                        found = true;
                        order->setStatus(OrderStatus::COLLECTING);
                        order->setCollectorId(volunteer->getId());
                        wareHouse.assignOrder(it);
                    }
                }
                ++volunteerIt;
            }
        }
        else if (order->getStatus() == OrderStatus::COLLECTING)
        {
            auto volunteerIt = wareHouse.getVolunteerVector().begin();

            while (!found && volunteerIt != wareHouse.getVolunteerVector().end())
            {
                auto volunteer = *volunteerIt;
                if (volunteer->type() == "Driver" || volunteer->type() == "LimitedDriver`")
                {
                    if (volunteer->canTakeOrder(*order))
                    {
                        volunteer->acceptOrder(*order);
                        found = true;
                        order->setStatus(OrderStatus::DELIVERING);
                        order->setDriverId(volunteer->getId());
                        wareHouse.assignOrder(it);
                    }
                }
                ++volunteerIt;
            }
        }
        if (!found)
            ++it;
    }
}
void SimulateStep::promoteOrders(WareHouse &wareHouse)
{
    for (auto volunteer : wareHouse.getVolunteerVector())
    {
        volunteer->step();
    }
}
void SimulateStep::freeUpVolunteers(WareHouse &wareHouse)
{
    for (auto volu : wareHouse.getVolunteerVector())
    {
        if (volu->hasJustFinishedJob())
        {

            int finishedId = volu->getCompletedOrderId();

            auto it = find_if(wareHouse.getInProgressVector().begin(), wareHouse.getInProgressVector().end(), [finishedId](const Order *order)
                              { return order != nullptr && order->getId() == finishedId; });

            wareHouse.moveFromVolunteerOrder(it);
        }
    }
}
void SimulateStep::fireVolunteers(WareHouse &wareHouse)
{
    vector<Volunteer *>::const_iterator it = wareHouse.getVolunteerVector().begin();
    while (it != wareHouse.getVolunteerVector().end())
    {
        auto vol = *it;
        if ((vol->type() == "LimitedDriver") | (vol->type() == "LimitedCollector"))
        {
            // auto castedVol = dynamic_cast<LimitedDriverVolunteer *>(*it);
            if (!vol->isBusy() && !vol->hasOrdersLeft())
            {
                wareHouse.fireVolunteer(it); // deletes from vector
                delete vol;
            }
        }

        ++it;
    }
}

/////////////////////////////////PrintActionsLog/////////////////////////

PrintActionsLog::PrintActionsLog() : BaseAction() {}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    for (auto action : wareHouse.getActions())
    {
        cout << action->toString() << endl;
    }
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
    return "log COMPLETED";
}

//////////////BackUp/////////////////////

BackupWareHouse::BackupWareHouse() : BaseAction() {}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    if (backup != nullptr)
        delete backup;
    backup = new WareHouse(wareHouse);
    complete();
}

string BackupWareHouse::toString() const
{
    return "backup COMPLETED";
}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}
///////////////////Restore///////////////////

RestoreWareHouse::RestoreWareHouse() : BaseAction() {}

void RestoreWareHouse::act(WareHouse &wareHouse)
{

    if (backup == nullptr)
    {

        error("No backup Available");
        cout << "ERROR: " << getErrorMsg() << endl;
    }
    else
    {
        wareHouse = *backup;
        complete();
    }
}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return "restore " + getErrorMsg() + " ERROR";
    return "restore COMPLETED";
}

/////////////////////Close///////////////////////
Close::Close() : BaseAction() {}

void Close::act(WareHouse &wareHouse)
{
    wareHouse.close();
    complete();
}

std::string Close::toString() const
{
    return "";
}
Close *Close::clone() const
{
    return new Close(*this);
}