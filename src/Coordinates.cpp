#include "Coordinates.h"
#include <cmath>     
#include <iomanip>   

using namespace std;

Coordinates::Coordinates() : latitude(0.0), longitude(0.0) {}

Coordinates::Coordinates(double latitude, double longitude) : latitude(latitude), longitude(longitude) {}

void Coordinates::setLatitude(double latitude)
{
    this->latitude = latitude;
}

void Coordinates::setLongitude(double longitude)
{
    this->longitude = longitude;
}

double Coordinates::getLatitude() const 
{
    return latitude; 
}

double Coordinates::getLongitude() const 
{ 
    return longitude; 
}

double Coordinates::distanceTo(const Coordinates& other) const 
{
    double latitudeDiff = latitude - other.latitude;
    double longitudeDiff = longitude - other.longitude;
    return sqrt(latitudeDiff * latitudeDiff + longitudeDiff * longitudeDiff);
}

ostream& operator<<(ostream& out, const Coordinates& c) 
{
    out << fixed << setprecision(4) << "(" << c.latitude << ", " << c.longitude << ")";
    return out;
}
