#include "Centroid.h"
#include <cmath>     
#include <iomanip>  
#include <iostream>

using namespace std;

const double EPSILON = 0.0001;

Centroid::Centroid()
{
    for (int i = 0; i < 3; i++)
        features[i] = 0.0;
}

Centroid::Centroid(double lat, double lon, double totalSales)
{
    features[0] = lat;
    features[1] = lon;
    features[2] = totalSales;
}

void Centroid::setFeature(int index, double value)
{
    if (index >= 0 && index < 3)
        features[index] = value;
}

double Centroid::getFeature(int index) const
{
    if (index >= 0 && index < 3)
        return features[index];
    return 0.0;
}

double Centroid::distanceTo(const Centroid& other) const
{
    double sumOfSquares = 0.0;

    for (int i = 0; i < 3; i++)
    {
        double diff = features[i] - other.features[i];
        sumOfSquares += diff * diff;
    }

    return sqrt(sumOfSquares);
}

bool Centroid::operator==(const Centroid& other) const
{
    for (int i = 0; i < 3; i++)
    {
        if (fabs(features[i] - other.features[i]) > EPSILON)
            return false;
    }
    return true;
}

bool Centroid::operator!=(const Centroid& other) const
{
    return !(*this == other);
}

ostream& operator<<(ostream& out, const Centroid& c)
{
    out << fixed << setprecision(4);
    out << "Centroid(lat=" << c.features[0]
        << ", lon=" << c.features[1]
        << ", sales=" << c.features[2] << ")";
    return out;
}