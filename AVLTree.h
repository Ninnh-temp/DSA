#ifndef AVLTREE_H
#define AVLTREE_H

#include "Student.h"

/**
 * AVLNode class representing a node in the AVL tree
 * Each node contains a Student object and tree structure information
 */
class AVLNode {
public:
    Student student;     // Student data
    AVLNode* left;       // Left child
    AVLNode* right;      // Right child
    int height;          // Height of the node

    AVLNode(const Student& s);
};

/**
 * AVLTree class implementing a self-balancing AVL tree
 * Uses student ID as the key for balancing
 */
class AVLTree {
private:
    AVLNode* root;

    // Helper functions for tree operations
    int height(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode* node, const Student& student, bool& inserted);
    AVLNode* deleteNode(AVLNode* node, int id, bool& deleted);
    AVLNode* minValueNode(AVLNode* node);
    AVLNode* search(AVLNode* node, int id);
    void inOrder(AVLNode* node);
    void destroyTree(AVLNode* node);
    int countNodes(AVLNode* node);

public:
    // Constructor and Destructor
    AVLTree();
    ~AVLTree();

    // Public interface
    bool insert(const Student& student);      // Insert a student (returns false if duplicate ID)
    bool deleteStudent(int id);                // Delete a student by ID (returns false if not found)
    Student* search(int id);                   // Search for a student by ID (returns nullptr if not found)
    void displayAll();                         // Display all students in sorted order (by ID)
    int getHeight();                           // Get tree height
    int getTotalStudents();                    // Get total number of students
    bool isEmpty();                            // Check if tree is empty
    bool updateStudent(int id, const std::string& newName); // Update student name
};

#endif // AVLTREE_H
