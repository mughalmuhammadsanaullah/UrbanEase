#include "Cluster.h"
#include <iostream>
#include <iomanip>
using namespace std;

const int INITIAL_CAPACITY = 16;


Cluster::Cluster() : clusterName("Unnamed Cluster"), stores(nullptr), storeCount(0), capacity(0), centroidIndex(-1)
{
    capacity = INITIAL_CAPACITY;
    stores = new Store * [capacity];

    for (int i = 0; i < capacity; i++)
        stores[i] = nullptr;
}

Cluster::Cluster(const string& name, int centroidIdx) : clusterName(name), stores(nullptr), storeCount(0), capacity(INITIAL_CAPACITY), centroidIndex(centroidIdx)
{
    stores = new Store * [capacity];
    for (int i = 0; i < capacity; i++)
        stores[i] = nullptr;
}

Cluster::Cluster(const Cluster& other) : clusterName(other.clusterName), storeCount(other.storeCount), capacity(other.capacity), centroidIndex(other.centroidIndex)
{
    stores = new Store * [capacity];

    for (int i = 0; i < capacity; i++)
        stores[i] = (i < storeCount) ? other.stores[i] : nullptr;
}

Cluster& Cluster::operator=(const Cluster& other)
{
    if (this == &other) return *this;

    delete[] stores;
    stores = nullptr;

    clusterName = other.clusterName;
    storeCount = other.storeCount;
    capacity = other.capacity;
    centroidIndex = other.centroidIndex;

    stores = new Store * [capacity];
    for (int i = 0; i < capacity; i++)
        stores[i] = (i < storeCount) ? other.stores[i] : nullptr;

    return *this;
}

Cluster::~Cluster()
{
    delete[] stores;
    stores = nullptr;
    storeCount = 0;
    capacity = 0;
}

void Cluster::resize()
{
    int newCapacity = capacity * 2;
    Store** newArray = new Store * [newCapacity];

    for (int i = 0; i < newCapacity; i++)
        newArray[i] = (i < storeCount) ? stores[i] : nullptr;

    delete[] stores;
    stores = newArray;
    capacity = newCapacity;
}

void Cluster::addStore(Store* store)
{
    if (store == nullptr) return;

    if (storeCount == capacity)
        resize();

    stores[storeCount] = store;
    storeCount++;
}

void Cluster::clear()
{
    for (int i = 0; i < storeCount; i++)
        stores[i] = nullptr;

    storeCount = 0;
}


string Cluster::getClusterName() const 
{
    return clusterName;
}

int Cluster::getStoreCount() const 
{
    return storeCount; 
}

int Cluster::getCentroidIndex() const 
{
    return centroidIndex; 
}

Store* Cluster::getStore(int index) const
{
    if (index >= 0 && index < storeCount)
        return stores[index];
    return nullptr;
}

void Cluster::setClusterName(const string& name)
{
    clusterName = name;
}

double Cluster::totalRevenue() const
{
    double total = 0.0;
    for (int i = 0; i < storeCount; i++)
    {
        if (stores[i] != nullptr)
            total += stores[i]->getAnalytics().totalSales();
    }
    return total;
}

double Cluster::averageSales() const
{
    if (storeCount == 0) return 0.0;
    return totalRevenue() / storeCount;
}

Cluster Cluster::operator+(const Cluster& other) const
{
    Cluster merged(clusterName + " + " + other.clusterName, -1);

    for (int i = 0; i < storeCount; i++)
        merged.addStore(stores[i]);

    for (int i = 0; i < other.storeCount; i++)
        merged.addStore(other.stores[i]);

    return merged;
}


ostream& operator<<(ostream& out, const Cluster& c)
{
    out << "====================================\n";
    out << "CLUSTER : " << c.clusterName << "\n";
    out << "------------------------------------\n";
    out << "  Store Count   : " << c.storeCount << "\n";
    out << fixed << setprecision(2);
    out << "  Total Revenue : " << c.totalRevenue() << "\n";
    out << "  Avg Per Store : " << c.averageSales() << "\n";
    out << "====================================\n";
    return out;
}