#include "Person.h"
#include <iostream>
#include <string>

using namespace std;
	
Person::Person()
{
	name = "";
	age = 0;
}

Person::Person(string name, int age) : name(name), age(age) {}

Person::Person(const Person& other) : name(other.name), age(other.age) {}

Person& Person::operator=(const Person& other)
{
	name = other.name;
	age = other.age;
	return *this;
}

ostream& operator<<(ostream& out, const Person& p)
{
	out << "Name: " << p.name << endl;
	out << "Age: " << p.age << endl;

	return out;
}

void Person::setName(string name)
{
	this->name = name;
}

void Person::setAge(int age)
{
	this->age = age;
}

string Person::getName() const
{
	return name;
}

int Person::getAge() const
{
	return age;
}

void Person::display() const
{
	cout << "Name: " << name << endl;
	cout << "Age: " << age << endl;
}

