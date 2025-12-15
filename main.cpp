#include <iostream>
#include <limits>
#include "AVLTree.h"
#include "Student.h"

using namespace std;

// Function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to get a valid integer input
int getIntInput() {
    int value;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

// Function to display the menu
void displayMenu() {
    cout << "\n=== Student Management System ===\n";
    cout << "1. Add Student\n";
    cout << "2. Remove Student\n";
    cout << "3. Search Student\n";
    cout << "4. Display All Students\n";
    cout << "5. Update Student\n";
    cout << "6. Display Tree Info\n";
    cout << "7. Exit\n";
    cout << "Enter choice: ";
}

// Function to add a student
void addStudent(AVLTree& tree) {
    int id;
    string name;

    cout << "Enter Student ID (5-digit number, 10000-99999): ";
    id = getIntInput();

    // Validate ID
    if (!Student::isValidId(id)) {
        cout << "Error: ID must be a 5-digit number between 10000 and 99999.\n";
        return;
    }

    cout << "Enter Student Name: ";
    getline(cin, name);

    // Check if name is empty
    if (name.empty()) {
        cout << "Error: Name cannot be empty.\n";
        return;
    }

    Student student(id, name);
    if (tree.insert(student)) {
        cout << "Successfully added student: ID=" << id << ", Name=\"" << name << "\"\n";
    } else {
        cout << "Error: A student with ID " << id << " already exists.\n";
    }
}

// Function to remove a student
void removeStudent(AVLTree& tree) {
    int id;

    cout << "Enter Student ID to remove: ";
    id = getIntInput();

    if (tree.deleteStudent(id)) {
        cout << "Successfully removed student with ID " << id << ".\n";
    } else {
        cout << "Error: Student with ID " << id << " not found.\n";
    }
}

// Function to search for a student
void searchStudent(AVLTree& tree) {
    int id;

    cout << "Enter Student ID to search: ";
    id = getIntInput();

    Student* student = tree.search(id);
    if (student) {
        cout << "Found: ";
        student->display();
        cout << "\n";
    } else {
        cout << "Student with ID " << id << " not found.\n";
    }
}

// Function to display all students
void displayAllStudents(AVLTree& tree) {
    cout << "\n=== All Students (sorted by ID) ===\n";
    tree.displayAll();
}

// Function to update a student
void updateStudent(AVLTree& tree) {
    int id;
    string newName;

    cout << "Enter Student ID to update: ";
    id = getIntInput();

    cout << "Enter new name: ";
    getline(cin, newName);

    if (newName.empty()) {
        cout << "Error: Name cannot be empty.\n";
        return;
    }

    if (tree.updateStudent(id, newName)) {
        cout << "Successfully updated student with ID " << id << ".\n";
    } else {
        cout << "Error: Student with ID " << id << " not found.\n";
    }
}

// Function to display tree information
void displayTreeInfo(AVLTree& tree) {
    cout << "\n=== Tree Information ===\n";
    cout << "Total Students: " << tree.getTotalStudents() << "\n";
    cout << "Tree Height: " << tree.getHeight() << "\n";
    
    if (tree.isEmpty()) {
        cout << "Tree Status: Empty\n";
    } else {
        cout << "Tree Status: Balanced (AVL Tree)\n";
    }
}

int main() {
    AVLTree tree;
    int choice;

    cout << "=== Student Management System ===\n";
    cout << "Using AVL Tree for efficient student record management\n";

    while (true) {
        displayMenu();
        choice = getIntInput();

        switch (choice) {
            case 1:
                addStudent(tree);
                break;
            case 2:
                removeStudent(tree);
                break;
            case 3:
                searchStudent(tree);
                break;
            case 4:
                displayAllStudents(tree);
                break;
            case 5:
                updateStudent(tree);
                break;
            case 6:
                displayTreeInfo(tree);
                break;
            case 7:
                cout << "Thank you for using Student Management System. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 7.\n";
        }
    }

    return 0;
}
