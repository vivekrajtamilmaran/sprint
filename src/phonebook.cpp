// Program to define a class Phonebook with default and parameterised constructor
#include <iostream>
#include <string>
using namespace std;

class Phonebook{
	string name , phonenumber ;

	public :
		Phonebook(){};
		Phonebook(string name , string phonenumber ){
			this->name = name;
			this->phonenumber = phonenumber ;
		};
};

