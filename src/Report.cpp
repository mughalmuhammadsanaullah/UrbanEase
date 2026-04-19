#include "Report.h"
#include <iostream>
#include <iomanip>
#include <cmath>   

using namespace std;

Report::Report(Store* stores, int count) : scores(nullptr), storeCount(count)
{
    if (count <= 0) return;

    buildScores(stores, count);
    computeWeightedScores();
    sortDescending();
    assignRanks();
}

Report::~Report()
{
    delete[] scores;
    scores = nullptr;
    storeCount = 0;
}

void Report::buildScores(Store* stores, int count)
{
    scores = new CompositeScore[count];

    for (int i = 0; i < count; i++)
    {
        const Analytics& a = stores[i].getAnalytics();

        double sales = a.annualSales();
        double growth = a.averageMonthlyGrowth();
        double profit = a.profitability();

        scores[i] = CompositeScore(&stores[i], sales, growth, profit);
    }
}

void Report::findBounds(double& salesMin, double& salesMax, double& growthMin, double& growthMax, double& profitMin, double& profitMax) const
{
    salesMin = salesMax = scores[0].getAnnualSales();
    growthMin = growthMax = scores[0].getGrowthRate();
    profitMin = profitMax = scores[0].getProfitability();

    for (int i = 1; i < storeCount; i++)
    {
        double s = scores[i].getAnnualSales();
        double g = scores[i].getGrowthRate();
        double p = scores[i].getProfitability();

        if (s < salesMin)  salesMin = s;
        if (s > salesMax)  salesMax = s;
        if (g < growthMin) growthMin = g;
        if (g > growthMax) growthMax = g;
        if (p < profitMin) profitMin = p;
        if (p > profitMax) profitMax = p;
    }

    if (fabs(salesMax - salesMin) < 0.0001) salesMax = salesMin + 1.0;
    if (fabs(growthMax - growthMin) < 0.0001) growthMax = growthMin + 1.0;
    if (fabs(profitMax - profitMin) < 0.0001) profitMax = profitMin + 1.0;
}

void Report::computeWeightedScores()
{
    double salesMin, salesMax, growthMin, growthMax, profitMin, profitMax;
    findBounds(salesMin, salesMax, growthMin, growthMax, profitMin, profitMax);

    for (int i = 0; i < storeCount; i++)
    {
        double normSales = (scores[i].getAnnualSales() - salesMin) / (salesMax - salesMin);

        double normGrowth = (scores[i].getGrowthRate() - growthMin) / (growthMax - growthMin);

        double normProfit = (scores[i].getProfitability() - profitMin) / (profitMax - profitMin);

        double ws = WEIGHT_PROFIT * normProfit + WEIGHT_SALES * normSales + WEIGHT_GROWTH * normGrowth;

        scores[i].setWeightedScore(ws);
    }
}


void Report::sortDescending()
{
    for (int i = 0; i < storeCount - 1; i++)
    {
        for (int j = 0; j < storeCount - i - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                CompositeScore temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}

void Report::assignRanks()
{
    int rank = 0;

    for (int i = 0; i < storeCount; i++)
    {
        scores[i].setRank(++rank);   
    }
}

int Report::getStoreCount() const
{
    return storeCount;
}

CompositeScore& Report::getScore(int index) const
{
    return scores[index];
}

Store* Report::getTopStore() const
{
    if (storeCount == 0) return nullptr;
    return scores[0].getStore();       
}

Store* Report::getBottomStore() const
{
    if (storeCount == 0) return nullptr;
    return scores[storeCount - 1].getStore();  
}

void Report::printHeader(ostream& out) const
{
    out << "============================================\n";
    out << "  PERFORMANCE RANKING REPORT               \n";
    out << "  Scoring Formula:                         \n";
    out << "    Profitability : " << (WEIGHT_PROFIT * 100) << "%\n";
    out << "    Annual Sales  : " << (WEIGHT_SALES * 100) << "%\n";
    out << "    Growth Rate   : " << (WEIGHT_GROWTH * 100) << "%\n";
    out << "  Total stores ranked: " << storeCount << "\n";
    out << "============================================\n";
}

void Report::printOneRow(ostream& out, const CompositeScore& cs) const
{
    Store* s = cs.getStore();
    if (s == nullptr) return;

    out << fixed << setprecision(4);

    out << "  " << setw(4) << right << cs.getRank()
        << "  " << setw(10) << left << s->getStoreID()
        << "  " << setw(14) << left << s->getCity()
        << "  " << setw(8) << right << fixed << setprecision(4)
        << cs.getWeightedScore()
        << "  " << setw(14) << right << fixed << setprecision(2)
        << cs.getAnnualSales()
        << "  " << setw(14) << right << cs.getProfitability()
        << "  " << setw(10) << right << cs.getGrowthRate()
        << "\n";
}

void Report::printTopN(ostream& out, int n) const
{
    int limit = (n < storeCount) ? n : storeCount;

    out << "\n  TOP " << limit << " STORES:\n";
    out << "  "
        << setw(4) << "Rank"
        << "  " << setw(10) << left << "Store ID"
        << "  " << setw(14) << left << "City"
        << "  " << setw(8) << right << "Score"
        << "  " << setw(14) << right << "Annual Sales"
        << "  " << setw(14) << right << "Profitability"
        << "  " << setw(10) << right << "Growth"
        << "\n";
    out << "  " << string(80, '-') << "\n";

    for (int i = 0; i < limit; i++)
        printOneRow(out, scores[i]);
}

void Report::printBottomN(ostream& out, int n) const
{
    int limit = (n < storeCount) ? n : storeCount;
    int start = storeCount - limit;

    out << "\n  BOTTOM " << limit << " STORES (Need Attention):\n";
    out << "  "
        << setw(4) << "Rank"
        << "  " << setw(10) << left << "Store ID"
        << "  " << setw(14) << left << "City"
        << "  " << setw(8) << right << "Score"
        << "  " << setw(14) << right << "Annual Sales"
        << "  " << setw(14) << right << "Profitability"
        << "  " << setw(10) << right << "Growth"
        << "\n";
    out << "  " << string(80, '-') << "\n";

    for (int i = start; i < storeCount; i++)
        printOneRow(out, scores[i]);
}

ostream& operator<<(ostream& out, const Report& r)
{
    r.printHeader(out);
    r.printTopN(out, 10);
    r.printBottomN(out, 5);

    out << "\n============================================\n";
    return out;
}