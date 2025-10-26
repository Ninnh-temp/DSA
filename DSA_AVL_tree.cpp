#include <iostream>
#include <algorithm>

class AVLNode {
public:
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int val) : key(val), left(nullptr), right(nullptr), height(1) {}
};
class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode* insert(AVLNode* node, int key) {
        if (!node)
            return new AVLNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1 + std::max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }
    void preOrder(AVLNode* node) {
        if (node) {
            std::cout << node->key << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }

    void inOrder(AVLNode* node) {
        if (node) {
            inOrder(node->left);
            std::cout << node->key << " ";
            inOrder(node->right);
        }
    }

    void printTree(AVLNode* node, int level = 0) {
        if (node) {
            printTree(node->right, level + 1);
            for (int i = 0; i < level; i++)
                std::cout << "    ";
            std::cout << "[" << node->key << " (h:" << node->height << ", b:" << getBalance(node) << ")]\n";
            printTree(node->left, level + 1);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }

    void preOrder() {
        preOrder(root);
    }

    void inOrder() {
        inOrder(root);
    }

    void printTree() {
        if (!root) {
            std::cout << "Tree is empty!\n";
            return;
        }
        printTree(root);
    }

    int getHeight() {
        return height(root);
    }
};
int main() {
    AVLTree tree;
    int choice, value;

    std::cout << "=== AVL Tree Self-Balancing Tester ===\n";
    std::cout << "Test case 1: Insert ascending numbers (10,20,30,40,50,25)\n";

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    std::cout << "\nPreorder: ";
    tree.preOrder();
    std::cout << "\nInorder: ";
    tree.inOrder();
    std::cout << "\nTree structure (value (height, balance)):\n";
    tree.printTree();
    std::cout << "\nTree Height: " << tree.getHeight() << "\n";

    while (true) {
        std::cout << "\n=== Menu ===\n";
        std::cout << "1. Insert value\n";
        std::cout << "2. Show tree structure\n";
        std::cout << "3. Show preorder traversal\n";
        std::cout << "4. Show inorder traversal\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter value to insert: ";
                std::cin >> value;
                tree.insert(value);
                std::cout << "Inserted " << value << "\n";
                break;
            case 2:
                tree.printTree();
                break;
            case 3:
                std::cout << "Preorder: ";
                tree.preOrder();
                std::cout << "\n";
                break;
            case 4:
                std::cout << "Inorder: ";
                tree.inOrder();
                std::cout << "\n";
                break;
            case 5:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice!\n";
        }
    }

    return 0;
}
