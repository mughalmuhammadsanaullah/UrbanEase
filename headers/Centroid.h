#pragma once

#include <iostream>
using namespace std;

class Centroid
{
private:
    double features[3];

public:

    Centroid();
    Centroid(double lat, double lon, double totalSales);


    void setFeature(int index, double value);
    double getFeature(int index) const;

    double distanceTo(const Centroid& other) const;

    bool operator==(const Centroid& other) const;
    bool operator!=(const Centroid& other) const;  

    friend ostream& operator<<(ostream& out, const Centroid& c);
};

