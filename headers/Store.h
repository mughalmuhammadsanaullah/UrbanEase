#pragma once

#include <string>
#include <fstream>
#include "Coordinates.h"
#include "Analytics.h"
#include "Forecast.h"
#include "Manager.h"

using namespace std;

class Store {

private:
    string storeID;
    string storeName;
    string city;

    Coordinates location;
    Analytics analytics;
    Forecast forecast; 
    Manager manager; 

    Employee* staff;  
    int staffCount;

public:

    //Constructors
    Store();
    Store(string id, string name, string city, double lat, double lon, const Manager& mgr, int numStaff = 0);
    Store(const Store& other);
    Store& operator=(const Store& other);
    ~Store();

    // Getters
    string getStoreID() const;
    string getStoreName() const;
    string getCity() const;
    int getStaffCount() const;

    Coordinates& getLocation();
    Analytics& getAnalytics();
    Forecast& getForecast();
    Manager& getManager();

    const Coordinates& getLocation() const;
    const Analytics& getAnalytics() const;
    const Forecast& getForecast() const;
    const Manager& getManager() const;

    void setStaff(int index, const Employee& e);
    Employee* getStaff(int index) const;

    void computeForecast();

    bool operator==(const Store& other) const;

    friend ostream& operator<<(ostream& out, const Store& s);

    //File operations
    void saveToFile(ofstream& out) const;
    bool loadFromFile(ifstream& in);
};

