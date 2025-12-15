#include "Student.h"
#include <iostream>

// Default constructor
Student::Student() : id(0), name("") {}

// Parameterized constructor
Student::Student(int id, const std::string& name) : id(id), name(name) {}

// Getters
int Student::getId() const {
    return id;
}

std::string Student::getName() const {
    return name;
}

// Setters
void Student::setId(int id) {
    this->id = id;
}

void Student::setName(const std::string& name) {
    this->name = name;
}

// Validate that ID is 5 digits (between 10000 and 99999)
bool Student::isValidId(int id) {
    return id >= 10000 && id <= 99999;
}

// Display student information
void Student::display() const {
    std::cout << id << " - " << name;
}
