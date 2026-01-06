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
 * Contains auto-assigned ID, name, and grade
 */
class Student {
private:
    int id;           // Auto-assigned student ID
    string name;      // Student name
    double grade;     // Student grade (0.0 - 100.0)

public:
    // Constructors
    Student() : id(0), name(""), grade(0.0) {}
    Student(int id, const string& name, double grade) : id(id), name(name), grade(grade) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    double getGrade() const { return grade; }

    // Setters
    void setId(int id) {
        this->id = id;
    }
    
    void setName(const string& name) {
        this->name = name;
    }

    void setGrade(double grade) {
        if (grade >= 0.0 && grade <= 100.0) {
            this->grade = grade;
        }
    }

    // Validate that grade is valid (between 0.0 and 100.0)
    static bool isValidGrade(double grade) {
        return grade >= 0.0 && grade <= 100.0;
    }

    // Display student information
    void display() const {
        cout << id << " - " << name << " (Grade: " << grade << ")";
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
 * Uses student name as the key for balancing
 */
class AVLTree {
private:
    AVLNode* root;
    int nextId;  // Auto-incrementing ID counter

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

        string studentName = student.getName();
        string nodeName = node->student.getName();

        if (studentName < nodeName) {
            node->left = insert(node->left, student, inserted);
        } else if (studentName > nodeName) {
            node->right = insert(node->right, student, inserted);
        } else {
            // Duplicate name - don't insert
            inserted = false;
            return node;
        }

        // 2. Update height of current node
        node->height = 1 + max(height(node->left), height(node->right));

        // 3. Get balance factor
        int balance = getBalance(node);

        // 4. Balance the tree if needed
        // Left Left Case
        if (balance > 1 && studentName < node->left->student.getName())
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && studentName > node->right->student.getName())
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && studentName > node->left->student.getName()) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && studentName < node->right->student.getName()) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Find node with minimum value (leftmost node)
    AVLNode* minValueNode(AVLNode* node) {
        if (!node)
            return nullptr;
        AVLNode* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    // Delete a node from the tree
    AVLNode* deleteNode(AVLNode* node, const string& name, bool& deleted) {
        if (!node) {
            deleted = false;
            return node;
        }

        // 1. Perform standard BST deletion
        if (name < node->student.getName()) {
            node->left = deleteNode(node->left, name, deleted);
        } else if (name > node->student.getName()) {
            node->right = deleteNode(node->right, name, deleted);
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
            node->right = deleteNode(node->right, temp->student.getName(), tempDeleted);
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

    // Search for a student by name
    AVLNode* search(AVLNode* node, const string& name) {
        if (!node || node->student.getName() == name)
            return node;

        if (name < node->student.getName())
            return search(node->left, name);
        else
            return search(node->right, name);
    }

    // In-order traversal (displays students in sorted order by name)
    void inOrder(AVLNode* node) {
        if (node) {
            inOrder(node->left);
            cout << "  ";
            node->student.display();
            cout << "\n";
            inOrder(node->right);
        }
    }

    // Search students by grade range
    void searchByGradeRange(AVLNode* node, double minGrade, double maxGrade, bool& found) {
        if (node) {
            searchByGradeRange(node->left, minGrade, maxGrade, found);
            
            double grade = node->student.getGrade();
            if (grade >= minGrade && grade <= maxGrade) {
                cout << "  ";
                node->student.display();
                cout << "\n";
                found = true;
            }
            
            searchByGradeRange(node->right, minGrade, maxGrade, found);
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

    // Helper function to display tree structure visually
    void displayTreeStructure(AVLNode* node, const string& prefix, bool isLeft, bool isRoot) {
        if (!node)
            return;

        cout << prefix;
        
        if (!isRoot) {
            cout << (isLeft ? "├── " : "└── ");
        }
        
        cout << node->student.getName() << " (ID: " << node->student.getId() << ", Grade: " << node->student.getGrade() << ")\n";
        
        // Update prefix for children
        string newPrefix = prefix;
        if (!isRoot) {
            newPrefix += (isLeft ? "│   " : "    ");
        }
        
        // Check if there are any children to display
        bool hasLeft = (node->left != nullptr);
        bool hasRight = (node->right != nullptr);
        
        // Display left child first (appears at top in visualization)
        if (hasLeft) {
            displayTreeStructure(node->left, newPrefix, hasRight, false);
        }
        
        // Display right child
        if (hasRight) {
            displayTreeStructure(node->right, newPrefix, false, false);
        }
    }

    // In-order traversal with step-by-step visualization
    void inOrderVisual(const AVLNode* node, int& step, string& path) {
        if (node) {
            inOrderVisual(node->left, step, path);
            
            cout << "Step " << step++ << ": Visiting node " 
                 << node->student.getName() << " (ID: " << node->student.getId() 
                 << ", Grade: " << node->student.getGrade() << ")\n";
            
            if (!path.empty()) {
                path += " → ";
            }
            path += node->student.getName();
            
            inOrderVisual(node->right, step, path);
        }
    }

    // Pre-order traversal with step-by-step visualization
    void preOrderVisual(const AVLNode* node, int& step, string& path) {
        if (node) {
            cout << "Step " << step++ << ": Visiting node " 
                 << node->student.getName() << " (ID: " << node->student.getId() 
                 << ", Grade: " << node->student.getGrade() << ")\n";
            
            if (!path.empty()) {
                path += " → ";
            }
            path += node->student.getName();
            
            preOrderVisual(node->left, step, path);
            preOrderVisual(node->right, step, path);
        }
    }

    // Post-order traversal with step-by-step visualization
    void postOrderVisual(const AVLNode* node, int& step, string& path) {
        if (node) {
            postOrderVisual(node->left, step, path);
            postOrderVisual(node->right, step, path);
            
            cout << "Step " << step++ << ": Visiting node " 
                 << node->student.getName() << " (ID: " << node->student.getId() 
                 << ", Grade: " << node->student.getGrade() << ")\n";
            
            if (!path.empty()) {
                path += " → ";
            }
            path += node->student.getName();
        }
    }

public:
    // Constructor
    AVLTree() : root(nullptr), nextId(10000) {}

    // Destructor
    ~AVLTree() {
        destroyTree(root);
    }

    // Insert a student (returns false if duplicate name)
    bool insert(const Student& student) {
        bool inserted = false;
        // Create a new student with auto-assigned ID
        Student newStudent(nextId, student.getName(), student.getGrade());
        root = insert(root, newStudent, inserted);
        if (inserted) {
            nextId++;  // Increment ID for next student
        }
        return inserted;
    }

    // Delete a student by name (returns false if not found)
    bool deleteStudent(const string& name) {
        bool deleted = false;
        root = deleteNode(root, name, deleted);
        return deleted;
    }

    // Search for a student by name (returns nullptr if not found)
    const Student* search(const string& name) {
        AVLNode* node = search(root, name);
        return node ? &(node->student) : nullptr;
    }

    // Display all students in sorted order (by name)
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

    // Update student grade
    bool updateStudent(const string& name, double newGrade) {
        AVLNode* node = search(root, name);
        if (node) {
            node->student.setGrade(newGrade);
            return true;
        }
        return false;
    }

    // Display students by grade range
    void displayByGradeRange(double minGrade, double maxGrade) {
        if (!root) {
            cout << "No students in the system.\n";
            return;
        }
        bool found = false;
        searchByGradeRange(root, minGrade, maxGrade, found);
        if (!found) {
            cout << "No students found in the grade range [" << minGrade << ", " << maxGrade << "].\n";
        }
    }

    // Display tree structure visually
    void displayTreeStructure() {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }
        cout << "\n=== Tree Structure ===\n";
        displayTreeStructure(root, "", false, true);
        cout << "\n";
    }

    // In-order traversal visualization (Left → Root → Right)
    void displayInOrderTraversal() {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }
        cout << "\n=== In-order Traversal (Left → Root → Right) ===\n";
        int step = 1;
        string path = "";
        inOrderVisual(root, step, path);
        cout << "\nTraversal Order: " << path << "\n\n";
    }

    // Pre-order traversal visualization (Root → Left → Right)
    void displayPreOrderTraversal() {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }
        cout << "\n=== Pre-order Traversal (Root → Left → Right) ===\n";
        int step = 1;
        string path = "";
        preOrderVisual(root, step, path);
        cout << "\nTraversal Order: " << path << "\n\n";
    }

    // Post-order traversal visualization (Left → Right → Root)
    void displayPostOrderTraversal() {
        if (!root) {
            cout << "Tree is empty.\n";
            return;
        }
        cout << "\n=== Post-order Traversal (Left → Right → Root) ===\n";
        int step = 1;
        string path = "";
        postOrderVisual(root, step, path);
        cout << "\nTraversal Order: " << path << "\n\n";
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
    cout << "5. Update Student Grade\n";
    cout << "6. Display Tree Info\n";
    cout << "7. Search by Grade Range\n";
    cout << "8. Visualize Tree Structure\n";
    cout << "9. Tree Traversal Visualization\n";
    cout << "10. Exit\n";
    cout << "Enter choice: ";
}

// Function to add a student
void addStudent(AVLTree& tree) {
    string name;
    double grade;

    cout << "Enter Student Name: ";
    getline(cin, name);

    // Check if name is empty
    if (name.empty()) {
        cout << "Error: Name cannot be empty.\n";
        return;
    }

    cout << "Enter Student Grade (0.0 - 100.0): ";
    while (!(cin >> grade)) {
        cout << "Invalid input. Please enter a valid grade (0.0 - 100.0): ";
        clearInputBuffer();
    }
    clearInputBuffer();

    // Validate grade
    if (!Student::isValidGrade(grade)) {
        cout << "Error: Grade must be between 0.0 and 100.0.\n";
        return;
    }

    Student student(0, name, grade);  // ID will be auto-assigned
    if (tree.insert(student)) {
        cout << "Successfully added student: Name=\"" << name << "\", Grade=" << grade << "\n";
    } else {
        cout << "Error: A student with name \"" << name << "\" already exists.\n";
    }
}

// Function to remove a student
void removeStudent(AVLTree& tree) {
    string name;

    cout << "Enter Student Name to remove: ";
    getline(cin, name);

    if (tree.deleteStudent(name)) {
        cout << "Successfully removed student \"" << name << "\".\n";
    } else {
        cout << "Error: Student with name \"" << name << "\" not found.\n";
    }
}

// Function to search for a student
void searchStudent(AVLTree& tree) {
    string name;

    cout << "Enter Student Name to search: ";
    getline(cin, name);

    const Student* student = tree.search(name);
    if (student) {
        cout << "Found: ";
        student->display();
        cout << "\n";
    } else {
        cout << "Student with name \"" << name << "\" not found.\n";
    }
}

// Function to display all students
void displayAllStudents(AVLTree& tree) {
    cout << "\n=== All Students (sorted by name) ===\n";
    tree.displayAll();
}

// Function to update a student
void updateStudent(AVLTree& tree) {
    string name;
    double newGrade;

    cout << "Enter Student Name to update: ";
    getline(cin, name);

    cout << "Enter new grade (0.0 - 100.0): ";
    while (!(cin >> newGrade)) {
        cout << "Invalid input. Please enter a valid grade (0.0 - 100.0): ";
        clearInputBuffer();
    }
    clearInputBuffer();

    if (!Student::isValidGrade(newGrade)) {
        cout << "Error: Grade must be between 0.0 and 100.0.\n";
        return;
    }

    if (tree.updateStudent(name, newGrade)) {
        cout << "Successfully updated grade for student \"" << name << "\".\n";
    } else {
        cout << "Error: Student with name \"" << name << "\" not found.\n";
    }
}

// Function to search students by grade range
void searchByGradeRange(AVLTree& tree) {
    double minGrade, maxGrade;

    cout << "Enter minimum grade: ";
    while (!(cin >> minGrade)) {
        cout << "Invalid input. Please enter a valid grade: ";
        clearInputBuffer();
    }
    clearInputBuffer();

    cout << "Enter maximum grade: ";
    while (!(cin >> maxGrade)) {
        cout << "Invalid input. Please enter a valid grade: ";
        clearInputBuffer();
    }
    clearInputBuffer();

    // Validate grade range
    if (!Student::isValidGrade(minGrade) || !Student::isValidGrade(maxGrade)) {
        cout << "Error: Grades must be between 0.0 and 100.0.\n";
        return;
    }

    if (minGrade > maxGrade) {
        cout << "Error: Minimum grade cannot be greater than maximum grade.\n";
        return;
    }

    cout << "\n=== Students with grades between " << minGrade << " and " << maxGrade << " ===\n";
    tree.displayByGradeRange(minGrade, maxGrade);
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

// Function to visualize tree structure
void visualizeTreeStructure(AVLTree& tree) {
    tree.displayTreeStructure();
}

// Function to display traversal menu and handle traversal visualization
void traversalVisualizationMenu(AVLTree& tree) {
    cout << "\n=== Traversal Visualization ===\n";
    cout << "1. In-order Traversal (Left → Root → Right)\n";
    cout << "2. Pre-order Traversal (Root → Left → Right)\n";
    cout << "3. Post-order Traversal (Left → Right → Root)\n";
    cout << "4. Show All Traversals\n";
    cout << "5. Back to Main Menu\n";
    cout << "Enter choice: ";
    
    int choice = getIntInput();
    
    switch (choice) {
        case 1:
            tree.displayInOrderTraversal();
            break;
        case 2:
            tree.displayPreOrderTraversal();
            break;
        case 3:
            tree.displayPostOrderTraversal();
            break;
        case 4:
            tree.displayInOrderTraversal();
            tree.displayPreOrderTraversal();
            tree.displayPostOrderTraversal();
            break;
        case 5:
            return;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 5.\n";
    }
}

// Function to load test data
void loadTestData(AVLTree& tree) {
    cout << "\n=== Loading Test Data ===\n";
    
    // Test data: 5 students with grades
    struct TestStudent {
        string name;
        double grade;
    };
    
    TestStudent testStudents[] = {
        {"Lan", 85.5},
        {"Tú", 92.0},
        {"Nam", 78.5},
        {"Minh", 88.0},
        {"Bách", 95.5}
    };
    
    int count = 0;
    for (const auto& ts : testStudents) {
        Student student(0, ts.name, ts.grade);  // ID will be auto-assigned
        if (tree.insert(student)) {
            cout << "  Added: " << ts.name << " (Grade: " << ts.grade << ")\n";
            count++;
        } else {
            cout << "  Warning: Could not add " << ts.name << " - duplicate name\n";
        }
    }
    
    cout << "\nTotal students loaded: " << count << "\n";
    cout << "=============================\n";
}

int main() {
    AVLTree tree;
    int choice;

    cout << "=== Student Management System ===\n";
    cout << "Using AVL Tree for efficient student record management\n";
    cout << "Tree is now balanced by student name\n";

    // Load test data
    loadTestData(tree);

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
                searchByGradeRange(tree);
                break;
            case 8:
                visualizeTreeStructure(tree);
                break;
            case 9:
                traversalVisualizationMenu(tree);
                break;
            case 10:
                cout << "Thank you for using Student Management System. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 10.\n";
        }
    }
}
