#include "Analytics.h"
#include <iostream>
#include <iomanip>
using namespace std;

Analytics::Analytics() 
{
    monthlySales = new double[MONTHS];
    monthlyCosts = new double[MONTHS];
    monthlyCustomers = new int[MONTHS];

    for (int i = 0; i < MONTHS; i++) 
    {
        monthlySales[i] = 0.0;
        monthlyCosts[i] = 0.0;
        monthlyCustomers[i] = 0;
    }
}

Analytics::Analytics(const Analytics& other) 
{
    monthlySales = new double[MONTHS];
    monthlyCosts = new double[MONTHS];
    monthlyCustomers = new int[MONTHS];

    for (int i = 0; i < MONTHS; i++) 
    {
        monthlySales[i] = other.monthlySales[i];
        monthlyCosts[i] = other.monthlyCosts[i];
        monthlyCustomers[i] = other.monthlyCustomers[i];
    }
}

// operator=
Analytics& Analytics::operator=(const Analytics& other) {
    if (this == &other) return *this;
    delete[] monthlySales;
    delete[] monthlyCosts;
    delete[] monthlyCustomers;

    monthlySales = new double[MONTHS];
    monthlyCosts = new double[MONTHS];
    monthlyCustomers = new int[MONTHS];

    for (int i = 0; i < MONTHS; i++) 
    {
        monthlySales[i] = other.monthlySales[i];
        monthlyCosts[i] = other.monthlyCosts[i];
        monthlyCustomers[i] = other.monthlyCustomers[i];
    }

    return *this;
}

Analytics::~Analytics()
{
    delete[] monthlySales;
    delete[] monthlyCosts;
    delete[] monthlyCustomers;

    monthlySales = nullptr;
    monthlyCosts = nullptr;
    monthlyCustomers = nullptr;
}
 
double& Analytics::operator[](int index) 
{
    if (index < 0) index = 0;
    if (index >= MONTHS) index = MONTHS - 1;

    return monthlySales[index];
}

const double& Analytics::operator[](int index) const 
{
    if (index < 0) index = 0;
    if (index >= MONTHS) index = MONTHS - 1;

    return monthlySales[index];
}

Analytics Analytics::operator+(const Analytics& other) const 
{
    Analytics result;
    for (int i = 0; i < MONTHS; i++) 
    {
        result.monthlySales[i] = monthlySales[i] + other.monthlySales[i];
        result.monthlyCosts[i] = monthlyCosts[i] + other.monthlyCosts[i];
        result.monthlyCustomers[i] = monthlyCustomers[i] + other.monthlyCustomers[i];
    }
    return result;
}


bool Analytics::operator<(const Analytics& other) const 
{
    return totalSales() < other.totalSales();
}

bool Analytics::operator>(const Analytics& other) const 
{
    return totalSales() > other.totalSales();
}

bool Analytics::operator==(const Analytics& other) const 
{
    return totalSales() == other.totalSales();
}

void Analytics::setSales(int month, double value) 
{ 
    if (month >= 0 && month < MONTHS) monthlySales[month] = value; 
}

void Analytics::setCost(int month, double value) 
{
    if (month >= 0 && month < MONTHS) monthlyCosts[month] = value; 
}

void Analytics::setCustomers(int month, int value) 
{ 
    if (month >= 0 && month < MONTHS) monthlyCustomers[month] = value; 
}

double Analytics::getSales(int month) const 
{ 
    return (month >= 0 && month < MONTHS) ? monthlySales[month] : 0; 
}

double Analytics::getCost(int month) const 
{ 
    return (month >= 0 && month < MONTHS) ? monthlyCosts[month] : 0; 
}

int Analytics::getCustomers(int month) const 
{
    return (month >= 0 && month < MONTHS) ? monthlyCustomers[month] : 0; 
}

double Analytics::totalSales() const 
{
    double sum = 0;
    for (int i = 0; i < MONTHS; i++) sum += monthlySales[i];
    return sum;
}

double Analytics::totalCosts() const 
{
    double sum = 0;
    for (int i = 0; i < MONTHS; i++) sum += monthlyCosts[i];
    return sum;
}

double Analytics::profitability() const 
{
    return totalSales() - totalCosts();
}


double Analytics::annualSales() const 
{
    double sum = 0;
    for (int i = MONTHS - 12; i < MONTHS; i++) sum += monthlySales[i];
    return sum;
}

// Average of (month[i] - month[i-1]) over last 12 months
double Analytics::averageMonthlyGrowth() const 
{
    double sum = 0;
    for (int i = MONTHS - 11; i < MONTHS; i++)
    {
        double current = monthlySales[i];
        double previous = monthlySales[i - 1];
        sum += current - previous;
    }
     
    return sum / 11.0;
}

int Analytics::totalCustomers() const 
{
    int sum = 0;
    for (int i = 0; i < MONTHS; i++) sum += monthlyCustomers[i];
    return sum;
}

ostream& operator<<(ostream& out, const Analytics& a) 
{
    out << fixed << setprecision(2);
    out << "  Total Sales: " << a.totalSales() << endl;
    out << "  Annual Sales: " << a.annualSales() << endl;
    out << "  Total Costs: " << a.totalCosts() << endl;
    out << "  Profitability: " << a.profitability() << endl;
    out << "  Avg Growth: " << a.averageMonthlyGrowth() << endl;
    out << "  Total Customers: " << a.totalCustomers() << endl;

    return out;
}