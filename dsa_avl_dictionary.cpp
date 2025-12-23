#include <iostream>
#include <string>

template <typename K, typename V>
class DictNode {
public:
    K key;
    V value;
    DictNode* left;
    DictNode* right;
    int height;

    DictNode(K k, V v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};

template <typename K, typename V>
class AVLDictionary {
private:
    DictNode<K, V>* root;
    int nodeCount;

    static int maxVal(int a, int b) {
        return (a > b) ? a : b;
    }

    int height(DictNode<K, V>* node) {
        return node ? node->height : 0;
    }

    int getBalance(DictNode<K, V>* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    DictNode<K, V>* rightRotate(DictNode<K, V>* y) {
        DictNode<K, V>* x = y->left;
        DictNode<K, V>* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = maxVal(height(y->left), height(y->right)) + 1;
        x->height = maxVal(height(x->left), height(x->right)) + 1;

        return x;
    }

    DictNode<K, V>* leftRotate(DictNode<K, V>* x) {
        DictNode<K, V>* y = x->right;
        DictNode<K, V>* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = maxVal(height(x->left), height(x->right)) + 1;
        y->height = maxVal(height(y->left), height(y->right)) + 1;

        return y;
    }

    DictNode<K, V>* insert(DictNode<K, V>* node, K key, V value) {
        if (!node) {
            nodeCount++;
            return new DictNode<K, V>(key, value);
        }

        if (key < node->key)
            node->left = insert(node->left, key, value);
        else if (key > node->key)
            node->right = insert(node->right, key, value);
        else {
            // Key exists, update value
            node->value = value;
            return node;
        }

        node->height = 1 + maxVal(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    DictNode<K, V>* minValueNode(DictNode<K, V>* node) {
        DictNode<K, V>* current = node;
        while (current->left)
            current = current->left;
        return current;
    }

    DictNode<K, V>* deleteNode(DictNode<K, V>* node, K key) {
        if (!node)
            return node;

        if (key < node->key)
            node->left = deleteNode(node->left, key);
        else if (key > node->key)
            node->right = deleteNode(node->right, key);
        else {
            if (!node->left) {
                DictNode<K, V>* temp = node->right;
                delete node;
                nodeCount--;
                return temp;
            } else if (!node->right) {
                DictNode<K, V>* temp = node->left;
                delete node;
                nodeCount--;
                return temp;
            }

            DictNode<K, V>* temp = minValueNode(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->key);
        }

        if (!node)
            return node;

        node->height = 1 + maxVal(height(node->left), height(node->right));

        int balance = getBalance(node);

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

    DictNode<K, V>* search(DictNode<K, V>* node, K key) const {
        if (!node || node->key == key)
            return node;

        if (key < node->key)
            return search(node->left, key);
        return search(node->right, key);
    }

    void inOrder(DictNode<K, V>* node) {
        if (node) {
            inOrder(node->left);
            std::cout << "  [" << node->key << ": " << node->value << "]\n";
            inOrder(node->right);
        }
    }

    void printTree(DictNode<K, V>* node, int level = 0) {
        if (node) {
            printTree(node->right, level + 1);
            for (int i = 0; i < level; i++)
                std::cout << "    ";
            std::cout << "[" << node->key << ":" << node->value << " (h:" << node->height << ", b:" << getBalance(node) << ")]\n";
            printTree(node->left, level + 1);
        }
    }

    void destroyTree(DictNode<K, V>* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    AVLDictionary() : root(nullptr), nodeCount(0) {}

    ~AVLDictionary() {
        destroyTree(root);
    }

    void insert(K key, V value) {
        root = insert(root, key, value);
    }

    void remove(K key) {
        root = deleteNode(root, key);
    }

    bool contains(K key) {
        return search(root, key) != nullptr;
    }

    const V* get(K key) const {
        DictNode<K, V>* node = search(root, key);
        return node ? &(node->value) : nullptr;
    }

    bool update(K key, V value) {
        DictNode<K, V>* node = search(root, key);
        if (node) {
            node->value = value;
            return true;
        }
        return false;
    }

    void printAll() {
        if (!root) {
            std::cout << "Dictionary is empty!\n";
            return;
        }
        std::cout << "Dictionary entries (sorted by key):\n";
        inOrder(root);
    }

    void printTree() {
        if (!root) {
            std::cout << "Dictionary is empty!\n";
            return;
        }
        std::cout << "Tree structure (key:value (height, balance)):\n";
        printTree(root);
    }

    int size() {
        return nodeCount;
    }

    bool isEmpty() {
        return nodeCount == 0;
    }

    int getHeight() {
        return height(root);
    }
};

int main() {
    AVLDictionary<std::string, std::string> dict;
    int choice;
    std::string key, value;

    std::cout << "=== AVL Tree Dictionary ===\n";
    std::cout << "Initializing with sample data...\n\n";

    // Insert sample data
    dict.insert("cat", "A small domesticated feline");
    dict.insert("dog", "A loyal domesticated canine");
    dict.insert("raccoon", "A nocturnal mammal with a masked face");
    dict.insert("rat", "A small rodent with a long tail");
    dict.insert("tiger", "A large striped wild cat");
    dict.insert("zebra", "A striped African equine");

    std::cout << "Initial dictionary:\n";
    dict.printAll();
    std::cout << "\nTree structure:\n";
    dict.printTree();
    std::cout << "\nDictionary size: " << dict.size() << "\n";
    std::cout << "Tree height: " << dict.getHeight() << "\n";

    while (true) {
        std::cout << "\n=== Dictionary Menu ===\n";
        std::cout << "1. Insert/Update entry\n";
        std::cout << "2. Search for key\n";
        std::cout << "3. Delete entry\n";
        std::cout << "4. Show all entries\n";
        std::cout << "5. Show tree structure\n";
        std::cout << "6. Show dictionary info\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::cout << "Enter key: ";
                std::getline(std::cin, key);
                if (key.empty()) {
                    std::cout << "Error: Key cannot be empty.\n";
                    break;
                }
                std::cout << "Enter value: ";
                std::getline(std::cin, value);
                dict.insert(key, value);
                std::cout << "Entry added/updated successfully.\n";
                break;
            }
            case 2: {
                std::cout << "Enter key to search: ";
                std::getline(std::cin, key);
                if (key.empty()) {
                    std::cout << "Error: Key cannot be empty.\n";
                    break;
                }
                const std::string* found = dict.get(key);
                if (found) {
                    std::cout << "Found: [" << key << ": " << *found << "]\n";
                } else {
                    std::cout << "Key '" << key << "' not found.\n";
                }
                break;
            }
            case 3: {
                std::cout << "Enter key to delete: ";
                std::getline(std::cin, key);
                if (key.empty()) {
                    std::cout << "Error: Key cannot be empty.\n";
                    break;
                }
                if (dict.contains(key)) {
                    dict.remove(key);
                    std::cout << "Entry deleted successfully.\n";
                } else {
                    std::cout << "Key '" << key << "' not found.\n";
                }
                break;
            }
            case 4:
                dict.printAll();
                break;
            case 5:
                dict.printTree();
                break;
            case 6:
                std::cout << "Dictionary size: " << dict.size() << "\n";
                std::cout << "Tree height: " << dict.getHeight() << "\n";
                std::cout << "Is empty: " << (dict.isEmpty() ? "Yes" : "No") << "\n";
                break;
            case 7:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice!\n";
        }
    }

    return 0;
}
