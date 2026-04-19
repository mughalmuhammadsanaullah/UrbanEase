#pragma once

#include <iostream>
using namespace std;

const int MONTHS = 24;

class Analytics {

private:
    double* monthlySales;
    double* monthlyCosts;
    int* monthlyCustomers;

public:

    //Constructor
    Analytics();
    Analytics(const Analytics& other);

    //Destructor
    ~Analytics();

    //Overloaded Operators
    double& operator[](int index);
    const double& operator[](int index) const;
    Analytics& operator=(const Analytics& other);
    Analytics operator+(const Analytics& other) const;
    bool operator<(const Analytics& other) const;
    bool operator>(const Analytics& other) const;
    bool operator==(const Analytics& other) const;

    // Getters
    double getSales(int month) const;
    double getCost(int month) const;
    int getCustomers(int month) const;

    // Setters
    void setSales(int month, double value);
    void setCost(int month, double value);
    void setCustomers(int month, int value);
    
    //Other Methods
    double totalSales() const;
    double totalCosts() const;
    double profitability() const;
    double annualSales() const;   
    double averageMonthlyGrowth() const;
    int totalCustomers() const;

    friend ostream& operator<<(ostream& out, const Analytics& a);
};


