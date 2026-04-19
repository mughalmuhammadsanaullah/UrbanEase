#include "Forecast.h"
#include <cmath>      
#include <iomanip>    
#include <iostream>

using namespace std;

Forecast::Forecast() : predictedSales(0.0), confidenceLevel(0.0), warningFlag(false), currentMonth(1), shortTermAvg(0.0), longTermAvg(0.0), momentum(0.0) {}

Forecast::Forecast(const double* salesData, int monthCount) : predictedSales(0.0),confidenceLevel(0.0), warningFlag(false), currentMonth(monthCount + 1),  shortTermAvg(0.0), longTermAvg(0.0), momentum(0.0)
{
    computeForecast(salesData, monthCount);
}

Forecast::Forecast(const Forecast& other) : predictedSales(other.predictedSales), confidenceLevel(other.confidenceLevel), warningFlag(other.warningFlag), currentMonth(other.currentMonth), shortTermAvg(other.shortTermAvg), longTermAvg(other.longTermAvg), momentum(other.momentum) {}

Forecast& Forecast::operator=(const Forecast& other)
{
    if (this == &other) return *this;   

    predictedSales = other.predictedSales;
    confidenceLevel = other.confidenceLevel;
    warningFlag = other.warningFlag;
    currentMonth = other.currentMonth;
    shortTermAvg = other.shortTermAvg;
    longTermAvg = other.longTermAvg;
    momentum = other.momentum;

    return *this;
}

double Forecast::getPredictedSales() const
{
    return predictedSales; 
}

double Forecast::getConfidenceLevel() const 
{
    return confidenceLevel; 
}

bool Forecast::getWarningFlag() const 
{
    return warningFlag; 
}

int Forecast::getCurrentMonth() const 
{
    return currentMonth; 
}

double Forecast::getMomentum() const 
{
    return momentum; 
}

double Forecast::movingAverage(const double* salesData, int monthCount, int window) const
{
    if (monthCount <= 0 || salesData == nullptr) return 0.0;

    // Clamp window to available data
    int actualWindow = (window < monthCount) ? window : monthCount;

    int start = monthCount - actualWindow;

    double sum = 0.0;
    for (int i = start; i < monthCount; i++)
        sum += salesData[i];

    return sum / actualWindow;
}

double Forecast::computeMomentum(const double* salesData, int monthCount) const
{
    if (monthCount < 2 || salesData == nullptr) return 0.0;

    int actualWindow = (SHORT_WINDOW < monthCount) ? SHORT_WINDOW : monthCount;
    int start = monthCount - actualWindow;

    if (actualWindow < 2) return 0.0;

    double firstVal = salesData[start];
    double lastVal = salesData[monthCount - 1];

    return (lastVal - firstVal) / (double)(actualWindow - 1);
}

double Forecast::computeConfidence(const double* salesData, int monthCount) const
{
    if (monthCount <= 0 || salesData == nullptr) return 0.0;

    int actualWindow = (SHORT_WINDOW < monthCount) ? SHORT_WINDOW : monthCount;
    int start = monthCount - actualWindow;

    //compute mean
    double mean = 0.0;
    for (int i = start; i < monthCount; i++)
        mean += salesData[i];
    mean /= actualWindow;

    if (fabs(mean) < 0.0001) return 0.0;   

    // compute variance
    double variance = 0.0;
    for (int i = start; i < monthCount; i++)
    {
        double diff = salesData[i] - mean;
        variance += diff * diff;
    }
    variance /= actualWindow;

    //standard deviation  CV
    double stdDev = sqrt(variance);
    double cv = (stdDev / fabs(mean)) * 100.0;

    //map CV to confidence (clamp to [0, 100])
    double confidence = 100.0 - cv;
    if (confidence < 0.0)   confidence = 0.0;
    if (confidence > 100.0) confidence = 100.0;

    return confidence;
}

void Forecast::computeForecast(const double* salesData, int monthCount)
{
    if (salesData == nullptr || monthCount <= 0)
    {
        predictedSales = 0.0;
        confidenceLevel = 0.0;
        warningFlag = true;
        return;
    }

    longTermAvg = movingAverage(salesData, monthCount, LONG_WINDOW);
    shortTermAvg = movingAverage(salesData, monthCount, SHORT_WINDOW);

    momentum = computeMomentum(salesData, monthCount);

    predictedSales = shortTermAvg + momentum;

    
    if (predictedSales < 0.0) predictedSales = 0.0;

    confidenceLevel = computeConfidence(salesData, monthCount);

    
    warningFlag = (predictedSales < longTermAvg) || (momentum < 0.0);
}


Forecast Forecast::operator+(const Forecast& other) const
{
    Forecast combined;

    combined.predictedSales = predictedSales + other.predictedSales;
    combined.confidenceLevel = (confidenceLevel + other.confidenceLevel) / 2.0;
    combined.warningFlag = warningFlag || other.warningFlag;
    combined.currentMonth = currentMonth;
    combined.shortTermAvg = shortTermAvg + other.shortTermAvg;
    combined.longTermAvg = longTermAvg + other.longTermAvg;
    combined.momentum = momentum + other.momentum;

    return combined;
}

ostream& operator<<(ostream& out, const Forecast& f)
{
    out << fixed << setprecision(2);
    out << "  ----------------------------------------\n";
    out << "  FORECAST — Month " << f.currentMonth << "\n";
    out << "  ----------------------------------------\n";
    out << "  Predicted Sales   : " << f.predictedSales << "\n";
    out << "  Confidence Level  : " << f.confidenceLevel << "%\n";
    out << "  Short-Term Avg    : " << f.shortTermAvg << "\n";
    out << "  Long-Term Avg     : " << f.longTermAvg << "\n";
    out << "  Monthly Momentum  : " << f.momentum
        << (f.momentum >= 0 ? "  (rising)" : "  (falling)") << "\n";

    
    if (f.warningFlag)
        out << "  *** WARNING: Sales decline predicted! ***\n";
    else
        out << "  Status            : On track\n";

    out << "  ----------------------------------------\n";
    return out;
}

Forecast& Forecast::operator++()
{
    currentMonth++;

    predictedSales += momentum;
    if (predictedSales < 0.0) predictedSales = 0.0;

    warningFlag = (predictedSales < longTermAvg) || (momentum < 0.0);

    confidenceLevel *= 0.95;
    if (confidenceLevel < 0.0) confidenceLevel = 0.0;

    return *this;
}

Forecast Forecast::operator++(int)
{
    Forecast before = *this;   
    ++(*this);                 
    return before;             
}

Forecast& Forecast::operator--()
{
    if (currentMonth > 1)
    {
        currentMonth--;
        predictedSales -= momentum;
        if (predictedSales < 0.0) predictedSales = 0.0;

        warningFlag = (predictedSales < longTermAvg) || (momentum < 0.0);

        confidenceLevel /= 0.95;
        if (confidenceLevel > 100.0) confidenceLevel = 100.0;
    }

    return *this;
}

Forecast Forecast::operator--(int)
{
    Forecast before = *this;
    --(*this);
    return before;
}