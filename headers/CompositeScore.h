#pragma once

#include "Store.h"

class CompositeScore
{
private:

    Store* store;

    double annualSales;       
    double growthRate;
    double profitability;    

    double weightedScore;

    int rank;

public:

    CompositeScore();
    CompositeScore(Store* store, double annualSales, double growthRate, double profitability);

    void setWeightedScore(double score);
    void setRank(int rank);

    Store* getStore() const;
    double getAnnualSales() const;
    double getGrowthRate() const;
    double getProfitability()const;
    double getWeightedScore()const;
    int getRank() const;

    bool operator<(const CompositeScore& other) const;
    bool operator>(const CompositeScore& other) const;
    bool operator==(const CompositeScore& other) const;
};