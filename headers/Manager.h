#pragma once
#include "Employee.h"

class Manager : public Employee
{
private:
    string department;
    double bonus;

public:

    //Constructors
    Manager();
    Manager(string name, int age, int id, double salary, string department, double bonus);
    Manager(const Manager& other);
    Manager& operator=(const Manager& other);

    //Destructor
    //~Manager();


    //Setters
    void setDepartment(string dept);
    void setBonus(double bonus);

    //Getters
    string getDepartment() const;
    double getBonus() const;

    void display() const;
};

