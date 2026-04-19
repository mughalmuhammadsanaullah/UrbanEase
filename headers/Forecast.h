#pragma once
#include <string>
#include <iostream>
using namespace std;

class Forecast
{
private:

    double predictedSales;      
    double confidenceLevel;     
    bool warningFlag;         
    int currentMonth;        

    double shortTermAvg;        
    double longTermAvg;         
    double momentum;           

    static const int SHORT_WINDOW = 3;   
    static const int LONG_WINDOW = 12;  

public:
   
    Forecast();
    Forecast(const double* salesData, int monthCount);

    Forecast(const Forecast& other);

 
    //~Forecast();

    Forecast& operator=(const Forecast& other);

    
    double getPredictedSales() const;
    double getConfidenceLevel() const;
    bool getWarningFlag() const;
    int getCurrentMonth() const;
    double getMomentum() const;

    void computeForecast(const double* salesData, int monthCount);

    Forecast operator+(const Forecast& other) const;

    friend ostream& operator<<(ostream& out, const Forecast& f);

    Forecast& operator++();
    Forecast operator++(int);

    Forecast& operator--();

    Forecast operator--(int);

private:
    
    double movingAverage(const double* salesData, int monthCount, int window) const;
    double computeMomentum(const double* salesData, int monthCount) const;

    double computeConfidence(const double* salesData, int monthCount) const;
};