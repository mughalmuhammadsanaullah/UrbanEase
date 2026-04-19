#include "Manager.h"
#include <iostream>

using namespace std;

Manager::Manager()
{
	department = "";
	bonus = 0.0;
}

Manager::Manager(string name, int age, int employeeID, double salary, string dept, double bon)
    : Employee(name, age, employeeID, salary), department(dept), bonus(bon) {}

Manager::Manager(const Manager& other) : Employee(other), department(other.department), bonus(other.bonus) {}

Manager& Manager::operator=(const Manager& other) 
{
    if (this == &other) return *this;

    Employee::operator=(other);

    this->department = other.department;
    this->bonus = other.bonus;

    return *this;
}

string Manager::getDepartment() const 
{
    return department; 
}

double Manager::getBonus() const 
{ 
    return bonus; 
}

void Manager::setDepartment(string department) 
{
    this->department = department;
}

void Manager::setBonus(double bonus) 
{
    this->bonus = bonus >= 0.0 ? bonus : 0.0;
}

void Manager::display() const 
{
    Employee::display();  
    cout << "Dept: " << department << endl;
    cout << "Bonus: " << bonus << endl;
}
