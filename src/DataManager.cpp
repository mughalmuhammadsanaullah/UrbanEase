#include "DataManager.h"
#include <fstream>
#include <iostream>
#include <cstdlib>   
#include <ctime>     

using namespace std;

static double randDouble(double min, double max) 
{
    return min + (max - min) * (rand() / (double)RAND_MAX);
}

static int randInt(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

static int max(int a, int b)
{
    return a > b ? a : b;
}

string intToString(int num) 
{
    if (num == 0) return "0";

    string result;

    while (num > 0) 
    {
        char digit = (num % 10) + 48;
        result = digit + result;      
        num /= 10;
    }

    return result;
}

Store generateStore(int index) 
{
    const string cities[] = { "Karachi", "Lahore", "Islamabad", "Faisalabad", "Rawalpindi", "Multan" };
    const double cityLat[] = { 24.86,  31.55,  33.72,  31.42,  33.60,  30.19 };
    const double cityLon[] = { 67.01,  74.35,  73.04,  73.08,  73.05,  71.47 };
    const int numCities = 6;

    int cityIndex = index % numCities;

    string storeID = "STR-" + intToString(1000 + index);
    string storeName = "UrbanEase " + cities[cityIndex] + " #" + intToString(index / numCities + 1);

    double lat = cityLat[cityIndex] + randDouble(-0.5, 0.5);
    double lon = cityLon[cityIndex] + randDouble(-0.5, 0.5);

    const string mgrNames[] = {"Danish Ali", "Mehwish Tariq", "Saad Mirza", "Rabia Khalid", "Imran Butt", "Sana Nadeem"};
    const string depts[] = { "Sales", "Operations", "Marketing", "Finance" };

    int mgrID = 1000 + index;
    Manager mgr(mgrNames[index % 6], randInt(28, 55), mgrID, randDouble(80000, 200000), depts[index % 4], randDouble(10000, 50000));

    int numStaff = randInt(2, 8);
    Store s(storeID, storeName, cities[cityIndex], lat, lon, mgr, numStaff);

    const string staffNames[] = { "Ahmed", "Nadia", "Tariq", "Hina", "Rizwan", "Sana", "Kamran", "Ayesha" };

    for (int i = 0; i < numStaff; i++)
    {
        int empID = 5000 + index * 10 + i;
        Employee e(staffNames[i % 8], randInt(20, 45), empID, randDouble(25000, 60000));
        s.setStaff(i, e);
    }

    double baseSales = randDouble(500000, 3000000);
    double baseCosts = baseSales * randDouble(0.3, 0.6);

    for (int m = 0; m < MONTHS; m++) 
    {
        double trend = baseSales + m * randDouble(-5000, 15000);

        double seasonal = ((m % 12) >= 9) ? trend * randDouble(0.2, 0.4) : 0.0;

        double noise = trend * randDouble(-0.1, 0.1);
        double sales = max(100000.0, trend + seasonal + noise);
        double costs = max(50000.0, baseCosts + baseCosts * randDouble(-0.05, 0.05));
        int customers = randInt((int)(sales / 800), (int)(sales / 300));

        s.getAnalytics().setSales(m, sales);
        s.getAnalytics().setCost(m, costs);
        s.getAnalytics().setCustomers(m, customers);
    }

    s.computeForecast();
    return s;
}

void saveAllStores(const Store* stores, int count, const char* file) 
{
    ofstream ofs(file);
    if (!ofs.is_open()) 
    {
        cout << "ERROR: Cannot open file for writing: " << file << "\n";
        return;
    }

    ofs << count << "\n";

    for (int i = 0; i < count; i++) 
    {
        ofs << "---STORE---\n";    
        stores[i].saveToFile(ofs);
    }

    ofs.close();
    cout << "Saved " << count << " stores to \"" << file << "\".\n";
}

Store* loadAllStores(const char* file, int& outCount) 
{
    ifstream ifs(file);

    if (!ifs.is_open()) 
    {
        cout << "File not found: " << file << "\n";
        outCount = 0;
        return nullptr;
    }

    int count = 0;
    if (!(ifs >> count) || count <= 0) 
    {
        cout << "ERROR: Invalid store count in file.\n";
        outCount = 0;
        return nullptr;
    }

    ifs.ignore();   

    Store* stores = new Store[count];
    int loaded = 0;

    for (int i = 0; i < count; i++) 
    {
        string marker;
        getline(ifs, marker);   

        if (!stores[i].loadFromFile(ifs)) 
        {
            cout << "WARNING: Failed to load store " << i << ". Stopping.\n";
            break;
        }

        loaded++;
    }

    ifs.close();
    outCount = loaded;
    return stores;
}