#pragma once

#include <iostream>
using namespace std;

class Coordinates 
{
private:
    double latitude;
    double longitude;

public:

    //Constructors
    Coordinates();
    Coordinates(double latitude, double longitude);

    //Setters
    void setLatitude(double latitude);
    void setLongitude(double longitude);

    //Getters
    double getLatitude()  const;
    double getLongitude() const;

    double distanceTo(const Coordinates& other) const;

    friend ostream& operator<<(ostream& out, const Coordinates& c);
};


