#include "CompositeScore.h"

CompositeScore::CompositeScore() : store(nullptr), annualSales(0.0), growthRate(0.0), profitability(0.0), weightedScore(0.0), rank(0) {}

CompositeScore::CompositeScore(Store* s, double sales, double growth, double profit) : store(s), annualSales(sales), growthRate(growth), profitability(profit), weightedScore(0.0), rank(0) {}

void CompositeScore::setWeightedScore(double score)
{
    weightedScore = score;
}

void CompositeScore::setRank(int r)
{
    rank = r;
}

Store* CompositeScore::getStore() const 
{
    return store; 
}

double CompositeScore::getAnnualSales() const 
{
    return annualSales; 
}

double CompositeScore::getGrowthRate() const 
{
    return growthRate; 
}

double CompositeScore::getProfitability() const 
{
    return profitability; 
}

double CompositeScore::getWeightedScore() const 
{
    return weightedScore; 
}

int CompositeScore::getRank() const 
{
    return rank; 
}

bool CompositeScore::operator<(const CompositeScore& other) const
{
    double diff = weightedScore - other.weightedScore;

    if (diff > 0.0001)  return false;   
    if (diff < -0.0001) return true;    

    double pdiff = profitability - other.profitability;
    if (pdiff > 0.0001)  return false;
    if (pdiff < -0.0001) return true;

    return growthRate < other.growthRate;
}

bool CompositeScore::operator>(const CompositeScore& other) const
{
    return other < *this;   
}

bool CompositeScore::operator==(const CompositeScore& other) const
{
    return !((*this < other) || (*this > other));
}