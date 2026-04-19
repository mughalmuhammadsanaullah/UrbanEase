#pragma once

#include "CompositeScore.h"
#include <iostream>

using namespace std;

const double WEIGHT_PROFIT = 0.50;
const double WEIGHT_SALES = 0.30;
const double WEIGHT_GROWTH = 0.20;

class Report
{
private:

    CompositeScore* scores;
    int storeCount;

    void buildScores(Store* stores, int count);

    void findBounds(double& salesMin, double& salesMax, double& growthMin, double& growthMax, double& profitMin, double& profitMax) const;

    void computeWeightedScores();

    void sortDescending();

    void assignRanks();

    void printHeader(ostream& out) const;
    void printTopN(ostream& out, int n) const;    
    void printBottomN(ostream& out, int n) const; 
    void printOneRow(ostream& out, const CompositeScore& cs) const;

public:

    Report(Store* stores, int count);

    ~Report();

    friend ostream& operator<<(ostream& out, const Report& r);

    int getStoreCount() const;
    CompositeScore& getScore(int index) const;

    Store* getTopStore() const;

    Store* getBottomStore() const;
};