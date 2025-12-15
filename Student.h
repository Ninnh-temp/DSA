#ifndef STUDENT_H
#define STUDENT_H

#include <string>

/**
 * Student class representing a student record
 * Contains a 5-digit ID (10000-99999) and a name
 */
class Student {
private:
    int id;           // 5-digit student ID (10000-99999)
    std::string name; // Student name

public:
    // Constructors
    Student();
    Student(int id, const std::string& name);

    // Getters
    int getId() const;
    std::string getName() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);

    // Validation
    static bool isValidId(int id);

    // Display
    void display() const;
};

#endif // STUDENT_H
