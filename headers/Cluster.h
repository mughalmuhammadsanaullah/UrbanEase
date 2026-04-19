#pragma once

#include "Store.h"
#include <iostream>
#include <string>

using namespace std;

class Cluster
{
private:

    string clusterName;

    Store** stores;
    int storeCount;  
    int capacity;    

    int centroidIndex;

    void resize();

public:

    Cluster();
    Cluster(const string& name, int centroidIdx);

    Cluster(const Cluster& other);
    Cluster& operator=(const Cluster& other);

    ~Cluster();

    void addStore(Store* store);

    string getClusterName() const;
    int getStoreCount() const;
    int getCentroidIndex() const;
    Store* getStore(int index) const;  

    double totalRevenue() const;
    double averageSales() const;

    void setClusterName(const string& name);

    void clear();

    Cluster operator+(const Cluster& other) const;

    friend ostream& operator<<(ostream& out, const Cluster& c);
};

