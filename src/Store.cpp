#include "Store.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


static Employee* copyStaff(const Employee* src, int count)
{
    if (count == 0 || src == nullptr)
        return nullptr;

    Employee* destination = new Employee[count];

    for (int i = 0; i < count; i++)
        destination[i] = src[i];

    return destination;
}

Store::Store() : location(), analytics(), forecast(), manager(), staff(nullptr), staffCount(0) {}

Store::Store(string id, string name, string c, double lat, double lon, const Manager& mgr, int numStaff) : storeID(id), storeName(name), city(c), location(lat, lon), analytics(), forecast(), manager(mgr), staff(nullptr), staffCount(0)
{
    if (numStaff > 0)
    {
        staff = new Employee[numStaff];
        staffCount = numStaff;
    }
}

Store::Store(const Store& other) : storeID(other.storeID), storeName(other.storeName), city(other.city), location(other.location), analytics(other.analytics), forecast(other.forecast), manager(other.manager), staffCount(other.staffCount)
{
    staff = copyStaff(other.staff, other.staffCount);
}

Store& Store::operator=(const Store& other)
{
    if (this == &other) return *this;

    delete[] staff;
    staff = nullptr;

    storeID = other.storeID;
    storeName = other.storeName;
    city = other.city;
    location = other.location;
    analytics = other.analytics;
    forecast = other.forecast;
    manager = other.manager;
    staffCount = other.staffCount;
    staff = copyStaff(other.staff, other.staffCount);

    return *this;
}

Store::~Store()
{
    delete[] staff;
    staff = nullptr;
}

string Store::getStoreID() const
{
    return storeID;
}

string Store::getStoreName() const
{
    return storeName;
}

string Store::getCity() const
{
    return city;
}

int Store::getStaffCount() const
{
    return staffCount;
}

Coordinates& Store::getLocation()
{
    return location;
}

Analytics& Store::getAnalytics()
{
    return analytics;
}

Forecast& Store::getForecast()
{
    return forecast;
}

Manager& Store::getManager()
{
    return manager;
}

const Coordinates& Store::getLocation() const
{
    return location;
}

const Analytics& Store::getAnalytics() const
{
    return analytics;
}

const Forecast& Store::getForecast() const
{
    return forecast;
}

const Manager& Store::getManager() const
{
    return manager;
}

void Store::setStaff(int index, const Employee& e)
{
    if (index >= 0 && index < staffCount)
        staff[index] = e;
}

Employee* Store::getStaff(int index) const
{
    if (index >= 0 && index < staffCount) return &staff[index];
    return nullptr;
}

void Store::computeForecast()
{
    double salesData[MONTHS];
    for (int i = 0; i < MONTHS; i++)
        salesData[i] = analytics.getSales(i);

    forecast.computeForecast(salesData, MONTHS);
}

bool Store::operator==(const Store& other) const
{
    return storeID == other.storeID;
}

ostream& operator<<(ostream& out, const Store& s)
{
    out << "--------------------------------------------\n"
        << "Store ID   : " << s.storeID << "\n"
        << "Name       : " << s.storeName << "\n"
        << "City       : " << s.city << "\n"
        << "Location   : " << s.location << "\n"
        << "Staff Count: " << s.staffCount << "\n"
        << "Manager    : "; s.manager.display();
    out << "\nAnalytics  :\n" << s.analytics
        << "\nForecast   :\n" << s.forecast
        << "\n--------------------------------------------\n";
    return out;
}

void Store::saveToFile(ofstream& out) const
{
    out << storeID << "\n"
        << storeName << "\n"
        << city << "\n"
        << location.getLatitude() << "\n"
        << location.getLongitude() << "\n";

    out << manager.getName() << "\n"
        << manager.getAge() << "\n"
        << manager.getEmployeeID() << "\n"
        << manager.getSalary() << "\n"
        << manager.getDepartment() << "\n"
        << manager.getBonus() << "\n";

    out << staffCount << "\n";

    for (int i = 0; i < staffCount; i++)
    {
        out << staff[i].getName() << "\n"
            << staff[i].getAge() << "\n"
            << staff[i].getEmployeeID() << "\n"
            << staff[i].getSalary() << "\n";
    }

    for (int i = 0; i < MONTHS; i++)
    {
        out << analytics.getSales(i) << " "
            << analytics.getCost(i) << " "
            << analytics.getCustomers(i) << "\n";
    }

    out << forecast.getPredictedSales() << "\n"
        << forecast.getConfidenceLevel() << "\n"
        << forecast.getWarningFlag() << "\n"
        << forecast.getCurrentMonth() << "\n";
}

bool Store::loadFromFile(ifstream& in)
{
    
    if (!getline(in, storeID)) return false;
    if (!getline(in, storeName)) return false;
    if (!getline(in, city)) return false;

    double lat, lon;
    if (!(in >> lat >> lon)) return false;
    in.ignore();
    location.setLatitude(lat);
    location.setLongitude(lon);


    string mgrName, mgrDept;
    int mgrAge, mgrID;
    double mgrSal, mgrBonus;

    if (!getline(in, mgrName)) return false;
    if (!(in >> mgrAge)) return false;
    in.ignore();
    if (!(in >> mgrID)) return false;
    in.ignore();
    if (!(in >> mgrSal)) return false;
    in.ignore();
    if (!getline(in, mgrDept)) return false;
    if (!(in >> mgrBonus)) return false;
    in.ignore();

    manager = Manager(mgrName, mgrAge, mgrID, mgrSal, mgrDept, mgrBonus);

    int sc;
    if (!(in >> sc)) return false; in.ignore();

    delete[] staff;
    staff = nullptr;

    staffCount = sc;

    if (staffCount > 0)
    {
        staff = new Employee[staffCount];

        for (int i = 0; i < staffCount; i++)
        {
            string eName;
            int eAge, eID;
            double eSal;
            if (!getline(in, eName)) return false;
            if (!(in >> eAge)) return false;
            in.ignore();
            if (!(in >> eID)) return false;
            in.ignore();
            if (!(in >> eSal)) return false;
            in.ignore();
            staff[i] = Employee(eName, eAge, eID, eSal);
        }
    }
    else
    {
        staff = nullptr;
    }

    for (int i = 0; i < MONTHS; i++)
    {
        double s, c; int cust;
        if (!(in >> s >> c >> cust)) return false;
        in.ignore();
        analytics.setSales(i, s);
        analytics.setCost(i, c);
        analytics.setCustomers(i, cust);
    }

    double ps, cl; int dwInt, fm;
    if (!(in >> ps >> cl >> dwInt >> fm)) return false;
    in.ignore();

    computeForecast();

    return true;
}