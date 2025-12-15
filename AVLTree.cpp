#include "AVLTree.h"
#include <iostream>
#include <algorithm>

// AVLNode constructor
AVLNode::AVLNode(const Student& s) : student(s), left(nullptr), right(nullptr), height(1) {}

// AVLTree constructor
AVLTree::AVLTree() : root(nullptr) {}

// AVLTree destructor
AVLTree::~AVLTree() {
    destroyTree(root);
}

// Destroy tree recursively (for cleanup)
void AVLTree::destroyTree(AVLNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

// Get height of a node
int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}

// Get balance factor of a node
int AVLTree::getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Right rotation
AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left rotation
AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

// Insert a student into the tree
AVLNode* AVLTree::insert(AVLNode* node, const Student& student, bool& inserted) {
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
    node->height = 1 + std::max(height(node->left), height(node->right));

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

// Public insert function
bool AVLTree::insert(const Student& student) {
    bool inserted = false;
    root = insert(root, student, inserted);
    return inserted;
}

// Find node with minimum value (leftmost node)
AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left)
        current = current->left;
    return current;
}

// Delete a node from the tree
AVLNode* AVLTree::deleteNode(AVLNode* node, int id, bool& deleted) {
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
    node->height = 1 + std::max(height(node->left), height(node->right));

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

// Public delete function
bool AVLTree::deleteStudent(int id) {
    bool deleted = false;
    root = deleteNode(root, id, deleted);
    return deleted;
}

// Search for a student by ID
AVLNode* AVLTree::search(AVLNode* node, int id) {
    if (!node || node->student.getId() == id)
        return node;

    if (id < node->student.getId())
        return search(node->left, id);
    else
        return search(node->right, id);
}

// Public search function
Student* AVLTree::search(int id) {
    AVLNode* node = search(root, id);
    return node ? &(node->student) : nullptr;
}

// In-order traversal (displays students in sorted order by ID)
void AVLTree::inOrder(AVLNode* node) {
    if (node) {
        inOrder(node->left);
        std::cout << "  ";
        node->student.display();
        std::cout << "\n";
        inOrder(node->right);
    }
}

// Public display function
void AVLTree::displayAll() {
    if (!root) {
        std::cout << "No students in the system.\n";
        return;
    }
    inOrder(root);
}

// Get tree height
int AVLTree::getHeight() {
    return height(root);
}

// Count nodes in tree
int AVLTree::countNodes(AVLNode* node) {
    if (!node)
        return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

// Get total number of students
int AVLTree::getTotalStudents() {
    return countNodes(root);
}

// Check if tree is empty
bool AVLTree::isEmpty() {
    return root == nullptr;
}

// Update student name
bool AVLTree::updateStudent(int id, const std::string& newName) {
    Student* student = search(id);
    if (student) {
        student->setName(newName);
        return true;
    }
    return false;
}
