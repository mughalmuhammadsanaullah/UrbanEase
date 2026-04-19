#include "Employee.h"
#include <iostream>

using namespace std;

Employee::Employee() : employeeID(0), salary(0.0), Person() {}

Employee::Employee(string name, int age, int id, double sal)
    : Person(name, age), employeeID(id), salary(sal) {}

Employee::Employee(const Employee& other) : Person(other), employeeID(other.employeeID), salary(other.salary) {}

Employee& Employee::operator=(const Employee& other) 
{
    if (this == &other) return *this;

    Person::operator=(other);   

    employeeID = other.employeeID;
    salary = other.salary;

    return *this;
}

int Employee::getEmployeeID() const 
{
    return employeeID; 
}
double Employee::getSalary() const
{
    return salary;
}

void Employee::setEmployeeID(int employeeID)
{
    this->employeeID = employeeID;
}

void Employee::setSalary(double salary) 
{
    this->salary = salary;
}

Employee& Employee::operator++() 
{
    salary += 5000.0;
    return *this;
}

Employee Employee::operator++(int) 
{
    Employee temp(*this);
    salary += 5000.0;
    return temp;
}

Employee& Employee::operator--() 
{
    salary = salary >= 5000.0 ? salary - 5000.0 : 0.0;
    return *this;
}

Employee Employee::operator--(int) 
{
    Employee temp(*this);
    salary -= 5000.0;
    return temp;
}

void Employee::display() const 
{
    Person::display();
    cout << "ID: " << employeeID << endl;
    cout << "Salary : " << salary << endl;
}