/**
 * Student Management System using AVL Tree
 * Single-file implementation as per requirements
 * All code contained in one main.cpp file
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

/**
 * Student class representing a student record
 * Contains a 5-digit ID (10000-99999) and a name
 */
class Student {
private:
    int id;           // 5-digit student ID (10000-99999)
    string name;      // Student name

public:
    // Constructors
    Student() : id(0), name("") {}
    Student(int id, const string& name) : id(id), name(name) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }

    // Setters
    void setId(int id) {
        if (isValidId(id)) {
            this->id = id;
        }
    }
    
    void setName(const string& name) {
        this->name = name;
    }

    // Validate that ID is 5 digits (between 10000 and 99999)
    static bool isValidId(int id) {
        return id >= 10000 && id <= 99999;
    }

    // Display student information
    void display() const {
        cout << id << " - " << name;
    }
};

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

    AVLNode(const Student& s) : student(s), left(nullptr), right(nullptr), height(1) {}
};

/**
 * AVLTree class implementing a self-balancing AVL tree
 * Uses student ID as the key for balancing
 */
class AVLTree {
private:
    AVLNode* root;

    // Get height of a node
    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    // Get balance factor of a node
    int getBalance(AVLNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // Right rotation
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // Left rotation
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    // Insert a student into the tree
    AVLNode* insert(AVLNode* node, const Student& student, bool& inserted) {
        // 1. Perform normal BST insertion
        if (!node) {
            inserted = true;
            return new AVLNode(student);
        }

        int studentId = student.getId();
        int nodeId = node->student.getId();

        if (studentId < nodeId) {
            node->left = insert(node->left, student, inserted);
        } else if (studentId > nodeId) {
            node->right = insert(node->right, student, inserted);
        } else {
            // Duplicate ID - don't insert
            inserted = false;
            return node;
        }

        // 2. Update height of current node
        node->height = 1 + max(height(node->left), height(node->right));

        // 3. Get balance factor
        int balance = getBalance(node);

        // 4. Balance the tree if needed
        // Left Left Case
        if (balance > 1 && studentId < node->left->student.getId())
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && studentId > node->right->student.getId())
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && studentId > node->left->student.getId()) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && studentId < node->right->student.getId()) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Find node with minimum value (leftmost node)
    AVLNode* minValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    // Delete a node from the tree
    AVLNode* deleteNode(AVLNode* node, int id, bool& deleted) {
        if (!node) {
            deleted = false;
            return node;
        }

        // 1. Perform standard BST deletion
        if (id < node->student.getId()) {
            node->left = deleteNode(node->left, id, deleted);
        } else if (id > node->student.getId()) {
            node->right = deleteNode(node->right, id, deleted);
        } else {
            // Node found - delete it
            deleted = true;

            // Node with only one child or no child
            if (!node->left) {
                AVLNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                AVLNode* temp = node->left;
                delete node;
                return temp;
            }

            // Node with two children: get inorder successor (smallest in right subtree)
            AVLNode* temp = minValueNode(node->right);
            
            // Copy the inorder successor's data to this node
            node->student = temp->student;
            
            // Delete the inorder successor
            bool tempDeleted = false;
            node->right = deleteNode(node->right, temp->student.getId(), tempDeleted);
        }

        if (!node)
            return node;

        // 2. Update height of current node
        node->height = 1 + max(height(node->left), height(node->right));

        // 3. Get balance factor
        int balance = getBalance(node);

        // 4. Balance the tree if needed
        // Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Search for a student by ID
    AVLNode* search(AVLNode* node, int id) {
        if (!node || node->student.getId() == id)
            return node;

        if (id < node->student.getId())
            return search(node->left, id);
        else
            return search(node->right, id);
    }

    // In-order traversal (displays students in sorted order by ID)
    void inOrder(AVLNode* node) {
        if (node) {
            inOrder(node->left);
            cout << "  ";
            node->student.display();
            cout << "\n";
            inOrder(node->right);
        }
    }

    // Destroy tree recursively (for cleanup)
    void destroyTree(AVLNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // Count nodes in tree
    int countNodes(AVLNode* node) {
        if (!node)
            return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

public:
    // Constructor
    AVLTree() : root(nullptr) {}

    // Destructor
    ~AVLTree() {
        destroyTree(root);
    }

    // Insert a student (returns false if duplicate ID)
    bool insert(const Student& student) {
        bool inserted = false;
        root = insert(root, student, inserted);
        return inserted;
    }

    // Delete a student by ID (returns false if not found)
    bool deleteStudent(int id) {
        bool deleted = false;
        root = deleteNode(root, id, deleted);
        return deleted;
    }

    // Search for a student by ID (returns nullptr if not found)
    Student* search(int id) {
        AVLNode* node = search(root, id);
        return node ? &(node->student) : nullptr;
    }

    // Display all students in sorted order (by ID)
    void displayAll() {
        if (!root) {
            cout << "No students in the system.\n";
            return;
        }
        inOrder(root);
    }

    // Get tree height
    int getHeight() {
        return height(root);
    }

    // Get total number of students
    int getTotalStudents() {
        return countNodes(root);
    }

    // Check if tree is empty
    bool isEmpty() {
        return root == nullptr;
    }

    // Update student name
    bool updateStudent(int id, const string& newName) {
        Student* student = search(id);
        if (student) {
            student->setName(newName);
            return true;
        }
        return false;
    }
};

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
}
