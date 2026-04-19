#pragma once

#include <iostream>
#include <string>

using std::string;
using std::ostream;

class Person {
private:
	string name;
	int age;
public:
	
	//Constructors
	Person();
	Person(string name, int age);
	Person(const Person& other);

	//Overloaded Operators
	Person& operator = (const Person& other);
	friend ostream& operator<<(ostream& out, const Person& p);

	//Destructor
	//virtual ~Person();

	//Setters
	void setName(string name);
	void setAge(int age);

	//Getters
	string getName() const;
	int getAge() const;

	//Other Methods
	virtual void display() const;
};
