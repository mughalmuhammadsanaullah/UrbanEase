#pragma once
#include "Person.h"

class Employee : public Person
{
private:
	int employeeID;
	double salary;
public:

	//Constructors
	Employee();
	Employee(string name, int age, int id, double sal);
	Employee(const Employee& other);
	Employee& operator=(const Employee& other);

	//Destructor
	//virtual ~Employee();

	//Setters
	void setEmployeeID(int employeeID);
	void setSalary(double salary);

	//Getters
	int getEmployeeID() const;
	double getSalary() const;

	//Overloaded Operators
	Employee& operator++();
	Employee operator++(int);
	Employee& operator--();
	Employee operator--(int);

	//Other Methods
	virtual void display() const;
};

